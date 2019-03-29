package main

import (
    "fmt"

    "github.com/MDSplus/mdsplus/mdsobjects/golang/mdsplus"
)

func main() {
    var err error
    var shot int
    var t mdsplus.Tree 
    
    t.Open("slwtest", 0, 0)
    
    shot, err = t.GetCurrentShotId()
    if err != nil {
        panic(err)
    }
    fmt.Printf("Shot Id: %v\n", shot)
    
    fmt.Println("Incrementing Shot Id")
    t.IncrementCurrentShotId()
    
    shot, err = t.GetCurrentShotId()
    if err != nil {
        panic(err)
    }
    fmt.Printf("Shot Id: %v\n", shot)

    fmt.Println("Setting Shot Id to 10")
    t.SetCurrentShotId(10)

    shot, err = t.GetCurrentShotId()
    if err != nil {
        panic(err)
    }
    fmt.Printf("Shot Id: %v\n", shot)
    
    fmt.Printf("Datafile Size: %v\n", t.GetDatafileSize())

    node := t.GetNode("acq423")
    fmt.Printf("Node: %v\n", node)
    
    t.Close()
}
