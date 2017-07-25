local M = {
    tbl = {},
    eye_tbl = {}
}

function M:add(key)
    if key%3 == 0 then
        self.tbl[key] = true
    else
        self.eye_tbl[key] = true
    end
end

function M:check(key)
    if key%3 == 0 then
        return self.tbl[key]
    else
        return self.eye_tbl[key]
    end
end

function M:load()
    self:_load("tbl/tbl", self.tbl)
    self:_load("tbl/eye_tbl", self.eye_tbl)
end

function M:dump(file, tbl)
    self:_dump("tbl/tbl", self.tbl)
    self:_dump("tbl/eye_tbl", self.eye_tbl)
end

function M:_load(file, tbl)
    local f = io.open(file, "r")
    while true do
        local line = f:read()
        if not line then
            break
        end

        tbl[tonumber(line)] = true
    end
    f:close()
end

function M:_dump(file, tbl)
    local f = io.open(file, "w+")
    for k,_ in pairs(tbl) do
        f:write(k.."\n")
    end
    f:close()
end

return M
