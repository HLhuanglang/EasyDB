package main

import (
	"fmt"

	easydb "github.com/HLhuanglang/EasyDB"
)

func main() {
	db, err := easydb.Open("./test.db")
	if err != nil {
		fmt.Println(err)
	}

	if err := db.Put([]byte("Holo"), []byte("24")); err != nil {
		fmt.Println(err)
	}

	age, err := db.Get([]byte("Holo"))
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(string(age))
}
