package ce2d_error

type err struct{
	num int8
	message string
	callback func(message, file string, no int, time string)
}

func createError(num int8, message string, callback func(message, file string, no int, time string)) *err {
	return &err{
		num: num,
		message: message,
		callback: callback,
    }
}