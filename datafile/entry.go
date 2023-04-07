package datafile

import "encoding/binary"

const EntryHeadSize = 12

// [--mark--][--KeySize--][--ValueSize--][key][value]
// 4:4:4:key:value
type Entry struct {
	Key       []byte
	Value     []byte
	KeySize   uint32
	ValueSize uint32
	Mark      uint32
}

func NewEntry(key, value []byte, mark uint32) Entry {
	return Entry{
		Key:       key,
		Value:     value,
		KeySize:   uint32(len(key)),
		ValueSize: uint32(len(value)),
		Mark:      mark,
	}
}

// 获取大小
func (e *Entry) GetSize() int64 {
	return int64(EntryHeadSize + e.KeySize + e.ValueSize)
}

// 解码
func Decode(buf []byte) (*Entry, error) {
	mk := binary.BigEndian.Uint32(buf[0:4])
	ks := binary.BigEndian.Uint32(buf[4:8])
	vs := binary.BigEndian.Uint32(buf[8:12])
	return &Entry{KeySize: ks, ValueSize: vs, Mark: mk}, nil
}

// 编码
func Encode(e *Entry) ([]byte, error) {
	buf := make([]byte, e.GetSize())
	binary.BigEndian.PutUint32(buf[0:4], e.Mark)
	binary.BigEndian.PutUint32(buf[4:8], e.KeySize)
	binary.BigEndian.PutUint32(buf[8:12], e.ValueSize)
	copy(buf[EntryHeadSize:EntryHeadSize+e.KeySize], e.Key)
	copy(buf[EntryHeadSize+e.KeySize:], e.Value)
	return buf, nil
}
