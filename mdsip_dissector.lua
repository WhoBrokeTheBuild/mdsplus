
local client_types = 
{
    [1] = "VMS",
    [2] = "IEEE",
    [3] = "Java",
    [4] = "VMSG",
    [7] = "CRAY IEEE",
    [8] = "CRAY"
}

local data_types = 
{
    [2] = "BU",
    [3] = "WU",
    [4] = "LU",
    [5] = "QU",
    [6] = "B",
    [7] = "W",
    [8] = "L",
    [9] = "Q",
    [10] = "F",
    [11] = "D",
    [12] = "FC",
    [13] = "DC",
    [14] = "T",
}

local mdsip = Proto("mdsip", "MDSplus IP Protocol")

local pf_msglen         = ProtoField.new("Message Length", "mdsip.msglen", ftypes.INT32)
local pf_status         = ProtoField.new("Status", "mdsip.status", ftypes.INT32)
local pf_length         = ProtoField.new("Length", "mdsip.length", ftypes.INT16)
local pf_nargs          = ProtoField.new("Number of Args", "mdsip.nargs", ftypes.UINT8)
local pf_descriptor_idx = ProtoField.new("Descriptor Index", "mdsip.descriptor_idx", ftypes.UINT8)
local pf_message_id     = ProtoField.new("Message ID", "mdsip.message_id", ftypes.UINT8)
local pf_ndims          = ProtoField.new("Number of DIMs", "mdsip.ndims", ftypes.UINT8)

local pf_dim0 = ProtoField.int32("mdsip.dim0", base.DEC)
local pf_dim1 = ProtoField.int32("mdsip.dim1", base.DEC)
local pf_dim2 = ProtoField.int32("mdsip.dim2", base.DEC)
local pf_dim3 = ProtoField.int32("mdsip.dim3", base.DEC)
local pf_dim4 = ProtoField.int32("mdsip.dim4", base.DEC)
local pf_dim5 = ProtoField.int32("mdsip.dim5", base.DEC)
local pf_dim6 = ProtoField.int32("mdsip.dim6", base.DEC)
local pf_dim7 = ProtoField.int32("mdsip.dim7", base.DEC)

local pf_dtype          = ProtoField.uint8("mdsip.dtype", "Data Type", base.DEC, data_types)
local pf_client_type    = ProtoField.int8("mdsip.client_type", "Client Type", base.DEC, client_types)

local pf_data = ProtoField.new("Data", "mdsip.data", ftypes.BYTES)

mdsip.fields = {
    pf_msglen, pf_status, pf_length, pf_nargs, pf_descriptor_idx, pf_message_id,
    pf_dtype, pf_client_type, pf_ndims, 
    pf_dim0, pf_dim1, pf_dim2, pf_dim3, pf_dim4, pf_dim5, pf_dim6, pf_dim7,
    pf_data,
}

local MDSIP_HDR_LEN = 48

function mdsip.dissector(tvbuf, pktinfo, root)
    pktinfo.cols.protocol:set("MDSIP")

    local pktlen = tvbuf:reported_length_remaining()

    local tree = root:add(mdsip, tvbuf:range(0, pktlen))

    tree:add_le(pf_msglen,      tvbuf:range(0, 4))
    tree:add_le(pf_status,      tvbuf:range(4, 4))
    tree:add_le(pf_length,      tvbuf:range(8, 2))
    tree:add(pf_nargs,          tvbuf:range(10, 1))
    tree:add(pf_descriptor_idx, tvbuf:range(11, 1))
    tree:add(pf_message_id,     tvbuf:range(12, 1))
    tree:add(pf_dtype,          tvbuf:range(13, 1))
    tree:add(pf_client_type,    tvbuf:range(14, 1))
    tree:add(pf_ndims,          tvbuf:range(15, 1))

    local dims = tree:add("DIMs")

    dims:add_le(pf_dim0, tvbuf:range(16, 4))
    dims:add_le(pf_dim1, tvbuf:range(20, 4))
    dims:add_le(pf_dim2, tvbuf:range(24, 4))
    dims:add_le(pf_dim3, tvbuf:range(28, 4))
    dims:add_le(pf_dim4, tvbuf:range(32, 4))
    dims:add_le(pf_dim5, tvbuf:range(36, 4))
    dims:add_le(pf_dim6, tvbuf:range(40, 4))
    dims:add_le(pf_dim7, tvbuf:range(44, 4))

    tree:add(pf_data, tvbuf:range(MDSIP_HDR_LEN, pktlen - MDSIP_HDR_LEN))

    -- local msglen = tvbuf:range(0, 4):int()
    -- pktinfo.cols.info:set("msglen=" .. msglen)

    return pktlen
end

DissectorTable.get("tcp.port"):add(8000, mdsip)
DissectorTable.get("tcp.port"):add(59022, mdsip)