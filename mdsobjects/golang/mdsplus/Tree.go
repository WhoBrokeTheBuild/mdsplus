package mdsplus

// #cgo CFLAGS: -I ../../../include
// #cgo LDFLAGS: -L ../../../lib -lTreeShr
// #include <treeshr.h>
// #include <stdlib.h>
import "C"
import (
	"fmt"
	"unsafe"
)

// Tree is an MDSplus Data Storage Hierarchy
type Tree struct {
	dbid *unsafe.Pointer
	tree *C.char
	shot C.int
}

func (n *TreeNode) String() string {
	if n.nid == 0 {
		return "NODEREF(*)"
	} else {
		return fmt.Sprintf("NODEREF(%v)", n.nid)
	}
}

func (t *Tree) Open(tree string, shot int, flags int) int {
	t.dbid = C.TreeCtx()
	t.tree = C.CString(tree)
	t.shot = C.int(shot)
	status := int(C._TreeOpen(t.dbid, t.tree, t.shot, C.int(flags)))
	return status
}

func (t *Tree) Close() int {
	C._TreeClose(t.dbid, t.tree, t.shot)
	C.free(unsafe.Pointer(t.tree))
	return 0
}

func (t *Tree) SetCurrentShotId(shot int) int {
	return int(C.TreeSetCurrentShotId(t.tree, C.int(shot)))
}

func (t *Tree) GetCurrentShotId() (int, error) {
	shot := int(C.TreeGetCurrentShotId(t.tree))
	if shot == 0 {
		return shot, fmt.Errorf("No current shot number set for this tree.")
	}
	return shot, nil
}

func (t *Tree) IncrementCurrentShotId() (int, error) {
	shot, err := t.GetCurrentShotId()
	if err != nil {
		return shot, err
	}
	return t.SetCurrentShotId(shot + 1), nil
}

func (t *Tree) GetDatafileSize() int64 {
	return int64(C._TreeGetDatafileSize(*t.dbid))
}

func (t *Tree) GetNode(name string) *TreeNode {
	cname := C.CString(name)
	nid := C.int32_t(0)
	C._TreeFindNode(*t.dbid, cname, &nid)
	C.free(unsafe.Pointer(cname))
	return &TreeNode{
		tree: t,
		nid:  nid,
	}
}

func TreeOpen(tree string, shot int, flags int) int {
	cs := C.CString(tree)
	status := int(C.TreeOpen(cs, C.int(shot), C.int(flags)))
	C.free(unsafe.Pointer(cs))
	return status
}

func TreeClose(tree string, shot int) int {
	cs := C.CString(tree)
	status := int(C.TreeClose(cs, C.int(shot)))
	C.free(unsafe.Pointer(cs))
	return status
}

func TreeSetCurrentShotId(tree string, shot int) int {
	cs := C.CString(tree)
	shot = int(C.TreeSetCurrentShotId(cs, C.int(shot)))
	C.free(unsafe.Pointer(cs))
	return shot
}

func TreeGetCurrentShotId(tree string) (int, error) {
	cs := C.CString(tree)
	shot := int(C.TreeGetCurrentShotId(cs))
	C.free(unsafe.Pointer(cs))
	if shot == 0 {
		return shot, fmt.Errorf("No current shot number set for this tree.")
	}
	return shot, nil
}

func TreeIncrementCurrentShotId(tree string) (int, error) {
	shot, err := TreeGetCurrentShotId(tree)
	if err != nil {
		return shot, err
	}
	return TreeSetCurrentShotId(tree, shot+1), nil
}

func TreeGetDatafileSize() int64 {
	return int64(C.TreeGetDatafileSize())
}
