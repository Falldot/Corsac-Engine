package ce2d_math

import (
	"github.com/Falldot/ce2d/pkg/ce2d_error"
)

type (
	hashMap struct {
		values   []*mapValue
		Size     int
		fullSize int
	}

	mapValue struct {
		exist bool
		key   interface{}
		value interface{}
	}
)

func CreateHashMap() *hashMap {
	return &hashMap{
		Size:     0,
		fullSize: 15,
	}
}

func createValue(hash, value interface{}) *mapValue {
	return &mapValue{
		exist: true,
		key:   hash,
		value: value,
	}
}

func (h *hashMap) addValue(key uint64, value interface{}) {
	
}

func (h *hashMap) addString(key string, value interface{}) {
	var newKey uint64 = 0
	for i := 8; i < len(key); i += 8 {
		var value [8]uint64
		for index := range value {
			if index < len(key) {
				value[index] = uint64(key[i-(8-index)])
			} else {
				value[index] = 0
			}
		}

		newKey += uint64(value[7]) | uint64(value[6])<<8 | uint64(value[5])<<16 | uint64(value[4])<<24 |
			uint64(value[3])<<32 | uint64(value[2])<<40 | uint64(value[1])<<48 | uint64(value[0])<<56
	}

	h.addValue(newKey, value)
}

func (h *hashMap) Add(key, value interface{}) {
	switch key.(type) {
	case string:
		h.addString(key.(string), value)
	case int, int8, int16, int32, int64, uint, uint8, uint16, uint32, uint64:
		h.addValue(key.(uint64), value)
	default:
		ce2d_error.Error("Не поддерживаемый тип ключа!")
	}
}
