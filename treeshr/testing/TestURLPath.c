
#include <stdio.h>
#include <stdlib.h>
#include <treeshr.h>

extern int TdiIntrinsic(opcode_t opcode, int narg, mdsdsc_t * list[], mdsdsc_xd_t * out_ptr);

int main(int argc, char ** argv)
{
    (void)argc;
    (void)argv;

    // // setenv("default_tree_path", "http://mfews-heidcamp:8080", TRUE);
    // setenv("default_tree_path", "http://mfedata-archives:8088", TRUE); // ;alcdata-archives::

    // TreeOpen("magnetics", 1160930043, TRUE);

    // int nid;
    // TreeFindNode("\\IP", &nid);

    // printf("%d\n", nid);

    // mdsdsc_xd_t data = MDSDSC_XD_INITIALIZER;
    // TreeGetRecord(nid, &data);

    // mdsdsc_xd_t deco = MDSDSC_XD_INITIALIZER;
    // TdiIntrinsic(OPC_DECOMPILE, 1, &data.pointer, &deco);

    // printf("%.*s\n", deco.pointer->length, deco.pointer->pointer);

    return 0;
}