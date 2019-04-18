package mdsplus

// #cgo CFLAGS: -I ../../../include
// #include <treeshr.h>
// #include <stdlib.h>
// #include <mdsshr.h>
import "C"

type TreeNode struct {
	tree *Tree
	nid  C.int32_t
}

func emptyXD() C.mdsdsc_xd_t {
	return C.mdsdsc_xd_t{
		length:   0,
		dtype:    C.DTYPE_DSC,
		class:    C.CLASS_XD,
		pointer:  nil,
		l_length: 0,
	}
}

//func (n *TreeNode) GetData() int {
//	xd := emptyXD()
//	status := int(C._TreeGetRecord(n.tree.dbid, n.nid, &xd))
//	if status > 0 {
//		return status
//	}
//	C.MdsFree1Dx(&xd, 0)
//	return status
//}
