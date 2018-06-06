local M = {
    tbl = {},
    eye_tbl = {}
}

function M:add(key, huxi)
    self.tbl[key] = huxi
end

function M:add_eye(key, huxi)
    self.eye_tbl[key] = huxi
end

function M:check(big_key, small_key)
    local key = string.format("%d-%d", big_key, small_key)
    local not_eye = (big_key+small_key)%3 == 0
    if not_eye then
        return self.tbl[key]
    end

    return self.eye_tbl[key]
end

function M:load()
    self:load_tbl()
    self:load_eye_tbl()
end

function M:dump()
    self:dump_tbl()
    self:dump_eye_tbl()
end

function M:load_tbl()
    self:_load("tbl/table", self.tbl)
end

function M:dump_tbl()
    self:_dump("tbl/table", self.tbl)
end

function M:load_eye_tbl()
    self:_load("tbl/eye_table", self.eye_tbl)
end

function M:dump_eye_tbl()
    self:_dump("tbl/eye_table", self.eye_tbl)
end

function M:_dump(file, tbl)
    local f = io.open(file, "w+")
    for key,huxi in pairs(tbl) do
        f:write(key.."="..huxi.."\n")
    end
    f:close()
end

function M:_load(file, tbl)
    local f = io.open(file, "r")
    while true do
        local str = f:read("*|")
        if not str then
            break
        end
        local split = string.find("str","=")
        if not split then
            break
        end
        local key = string.sub(str, 1, split-1)
        local huxi = tonumber(string.sub(str, split+1))
        tbl[key] = huxi
    end
end

return M
