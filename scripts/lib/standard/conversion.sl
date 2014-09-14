// Conversion functions

func to_bool(val) {
	asm var val
	val = asm value [[
		tbool
	]]
	return val
}

func to_number(val) {
	asm var val
	val = asm value [[
		tnum
	]]
	return val
}

func to_string(val) {
	asm var val
	val = asm value [[
		tstr
	]]
	return val
}

func to_address(val) {
	asm var val
	val = asm value [[
		taddr
	]]
	return val
}

func type_id(val) {
	asm var val
	val = asm value [[
		type
	]]
	return val
}