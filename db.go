package easydb

import (
	"fmt"
	"io"
	"sync"

	"github.com/HLhuanglang/EasyDB/datafile"
)

type DB struct {
	dbFile  *datafile.DBFile
	hashidx map[string]int64
	mtx     sync.RWMutex
}

const (
	PUT uint16 = iota
	DEL
)

func Open(path string) (*DB, error) {
	// 加载数据文件
	dbFile, err := datafile.NewDBFile(path)
	if err != nil {
		return nil, err
	}

	db := &DB{
		dbFile:  dbFile,
		hashidx: make(map[string]int64),
	}

	// 加载索引
	db.loadIndexesFromFile()
	return db, nil
}

func (db *DB) Put(key []byte, val []byte) error {
	if len(key) == 0 {
		return nil
	}

	db.mtx.Lock()
	defer db.mtx.Unlock()

	//因为write操作后会更新Offset
	begin_offset := db.dbFile.Offset

	e := datafile.NewEntry(key, val, uint32(PUT))
	err := db.dbFile.Write(&e)
	if err != nil {
		return err
	}
	db.hashidx[string(key)] = begin_offset

	return nil
}

func (db *DB) Get(key []byte) ([]byte, error) {
	if len(key) == 0 {
		return nil, nil
	}

	db.mtx.RLock()
	defer db.mtx.RUnlock()

	offset, ok := db.hashidx[string(key)]
	if !ok {
		err := fmt.Errorf("{key: %s doesn't exit.}", key)
		return nil, err
	}
	entry, err := db.dbFile.Read(offset)
	if err != nil {
		return nil, nil
	}
	return entry.Value, nil
}

func (db *DB) loadIndexesFromFile() {
	if db.dbFile == nil {
		return
	}

	var offset int64
	for {
		e, err := db.dbFile.Read(offset)
		if err != nil {
			// 读取完毕
			if err == io.EOF {
				return
			}
			return
		}

		// 设置索引状态
		db.hashidx[string(e.Key)] = offset

		if uint16(e.Mark) == DEL {
			// 删除内存中的 key
			delete(db.hashidx, string(e.Key))
		}

		offset += e.GetSize()
	}
}
