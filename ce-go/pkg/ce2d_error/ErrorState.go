package ce2d_error

type state struct {
	errors map[int8]*err
}

func createState() *state{ 
	return &state{
		errors: make(map[int8]*err),
	}
}

func Add(num int8, message string, callback func(message, file string, no int, time string)){
	errorManager.errors[num] = createError(num, message, callback)
}