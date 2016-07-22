--[[

  This is some idiotic code which generates a C-style 24-bit look-up table for
  the WS2812's "NZR" representation of all bytes 0..255

  The generate_{bit,zero,one} functions are not used currently but would make it
  easier to automatically produce bit pulses of arbitrary widths.

  Right now, each bit "tile" is hand-counted as NZR_ZERO and NZR_ONE.

--]]

local bit = require('bit')

local NZR_ZERO = '111111110000000000000000'
local NZR_ONE  = '111111111111111100000000'

local NZR_ZERO_HIGH = 8
local NZR_ZERO_LOW = 17

local NZR_ONE_HIGH = 16
local NZR_ONE_LOW = 9

local generate_bit = function(pulse_width, high_bits, low_bits)

  local total_bits = high_bits + low_bits

  local n_ones = pulse_width * high_bits / total_bits
  local n_zeroes = pulse_width * low_bits / total_bits

  local bitstring = ''

  for _=1,n_ones do
    bitstring = bitstring .. '1'
  end
  for _=1,n_zeroes do
    bitstring = bitstring .. '0'
  end

  return bitstring

end

local generate_zero = function(pulse_width)
  return generate_bit(pulse_width, NZR_ZERO_HIGH, NZR_ZERO_LOW)
end

local generate_one = function(pulse_width)
  return generate_bit(pulse_width, NZR_ONE_HIGH, NZR_ONE_LOW)
end

local function get_bit(byte, position)
  return bit.band(0x01, bit.rshift(byte, position))
end

local bit_nzr = function(b)
  if b == 1 then
    return NZR_ONE
  else
    return NZR_ZERO
  end
end

local byte_nzr = function(byte)
  local line = ''
  for i=0,7 do
    local b = get_bit(byte, 7 - i)
    line = line .. bit_nzr(b)
  end
  return line
end

local bitbuf_append_byte = function(bytes, bytes_len, byte)
  bytes[bytes_len] = bit.band(0xff, byte)
end

local bitbuf_append_bits = function(bitbuf, bits)

  for c in bits:gmatch"." do

    local extra_bits = bit.lshift(bitbuf.bits, 1)

    if c == '1' then
      extra_bits = bit.bor(0x01, extra_bits)
    end

    if get_bit(extra_bits, 8) == 1 then
      local n_bytes = bitbuf.n_bytes + 1
      bitbuf_append_byte(bitbuf.bytes, n_bytes, extra_bits)
      bitbuf.n_bytes = n_bytes
      bitbuf.bits = 0x01
    else
      bitbuf.bits = extra_bits
    end

  end

  -- print(bits)

end

local main = function()
  for i=0,0xff do

    --[[
    since we're using bit shifting, the leading 1 of bits is actually a
    sentinel-type value. If the 9th bit from the right of an integer is 1, we've
    hit the 8th bit and must load on the full byte to bytes
    --]]

    local bitbuf = {
      bytes={},
      n_bytes=0,
      bits=0x01,
    }

    local line = byte_nzr(i)

    bitbuf_append_bits(bitbuf, line)

    io.write('{ ')

    for _, byte in ipairs(bitbuf.bytes) do
      io.write('0x' .. bit.tohex(byte, 2) .. ', ')
    end

    io.write('},\n')

  end
end

return {
  generate_zero=generate_zero,
  generate_one=generate_one,
  main=main,
}
