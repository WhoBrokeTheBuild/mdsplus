local MDSplus = require 'MDSplus'

function printkv(o) 
    print(o)
    for key,value in pairs(o) do
        print(key, value)
    end
end

print()

t = MDSplus.Tree.open("main", 0)
print(t)

print()

node = t:getNode("text")
print(node)
print(node:isOn())
data = node:getRecord()
print(data)

print()

node = t:getNode("numeric")
print(node)
print(node:isOn())
data = node:getRecord()
print(data)

print()

node = t:getNode("member")
print(node)
print(node:isOn())
data = node:getRecord()
for i,d in ipairs(data) do
    print(string.format("%d = %d", i, d))
end

print()

node = t:getNode("signal")
print(node)
print(node:isOn())
--data = node:getRecord()
--print(data)

print()

t:close()