package datafile

import "os"

type DBFile struct {
	File   *os.File
	Offset int64 //记录当前写入文件的偏移量,永远是指向文件末尾的。
}

func NewDBFile(path string) (*DBFile, error) {
	file, err := os.OpenFile(path, os.O_CREATE|os.O_RDWR, 0644)
	if err != nil {
		return nil, err
	}
	st, err := os.Stat(path)
	if err != nil {
		return nil, err
	}
	return &DBFile{File: file, Offset: st.Size()}, nil
}

// 不保证线程安全,需要外层调用确保
func (dbFile *DBFile) Write(e *Entry) error {
	buf, err := Encode(e)
	if err != nil {
		return err
	}
	_, err = dbFile.File.WriteAt(buf, dbFile.Offset)
	if err != nil {
		return nil
	}
	dbFile.Offset += e.GetSize()
	return nil
}

func (dbFile *DBFile) Read(offset int64) (*Entry, error) {
	buf := make([]byte, EntryHeadSize)
	_, err := dbFile.File.ReadAt(buf, offset)
	if err != nil {
		return nil, err
	}
	e, err := Decode(buf)
	if err != nil {
		return nil, nil
	}
	new_offset := offset + int64(EntryHeadSize)
	if e.KeySize > 0 {
		key := make([]byte, e.KeySize)
		if _, err := dbFile.File.ReadAt(key, new_offset); err != nil {
			return nil, err
		}
		e.Key = key
	}
	new_offset += int64(e.KeySize)
	if e.ValueSize > 0 {
		val := make([]byte, e.ValueSize)
		if _, err := dbFile.File.ReadAt(val, new_offset); err != nil {
			return nil, err
		}
		e.Value = val
	}
	return e, nil
}
