package ce2d_error

import ( 
	"fmt"
	"os"
	"runtime"
	"time"
)

var errorManager *state

func Init(){
	errorManager = createState()
}

func ThrowError(num int8){
	_, file, no, _ := runtime.Caller(1)
	t :=  time.Now().Format(time.RFC3339)
	errorManager.errors[num].callback(errorManager.errors[num].message, file, no, t)
}

func Error(text string) {
	_, file, no, _ := runtime.Caller(1)
	fmt.Println(text)
	fmt.Printf("Вызвана из %s#%d\n", file, no)
	os.Exit(1)
}