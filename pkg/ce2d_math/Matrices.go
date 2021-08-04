package ce2d_math

import (
	"github.com/Falldot/ce2d/pkg/ce2d_error"
)

type Matrix struct {
	width, height int
	value []int
}

func CreateMatrix(width, height int) *Matrix {
	return &Matrix{
        width: width,
        height: height,
        value: make([]int, width*height),
    }
}

func CreateMatrixWithValue(width, height int, value []int) *Matrix {
	if(len(value) <= width*height){
		ce2d_error.Error("Передаваемый массив больше размера матрицы ")
	}
	return &Matrix{
		width: width,
		height: height,
		value: value,
	}
}

func (m *Matrix) Add(value []int){
	if(len(value) > m.width*m.height){
		ce2d_error.Error("Передаваемый массив больше размера матрицы ")
	}
	m.value = value
}

func (m *Matrix) Get(w, h int) int{
	if(m.width <= w || m.height <= h){
		ce2d_error.Error("Передаваемые значения больше размера матрицы ")
	}
	return m.value[m.width*h+w]
}

func (m *Matrix) Set(w, h, value int) {
	if(m.width <= w || m.height <= h){
		ce2d_error.Error("Передаваемые значения больше размера матрицы ")
	}
	m.value[m.width*h+w] = value
}

func (m *Matrix) ForEach(callback func(index, width, height int)) {
	for i, v := range m.value {
		callback(v, i%m.width, i/m.height);
	}
}

func (m *Matrix) Map(callback func(index, width, height int) int) *Matrix {
	nMatrix := make([]int, m.width*m.height)
	for i, v := range m.value {
		nMatrix[i] = callback(v, i%m.width, i/m.height);
	}
	return CreateMatrixWithValue(m.width, m.height, nMatrix)
}

func (m *Matrix) Update(callback func(index, width, height int) int) {
	for i, v := range m.value {
		m.value[i] = callback(v, i%m.width, i/m.height);
	}
}

func (m *Matrix) Sum(other Matrix) *Matrix{
	if(m.width != other.width || m.height != other.height){
		ce2d_error.Error("Матрицы не равны по размеру ")
	}
	return m.Map(func(value, w, h int) int {
		return other.Get(w, h) + value
	})
}

func (m *Matrix) Sub(other Matrix) *Matrix{
	if(m.width != other.width || m.height != other.height){
		ce2d_error.Error("Матрицы не равны по размеру ")
	}

	return m.Map(func(value, w, h int) int {
		return  value - other.Get(w, h)
	})
}

func (m *Matrix) Mult(other Matrix) *Matrix{
	if(m.width != other.height){
		ce2d_error.Error("У первой матрицы ширина не равна высоте второй матрицы ")
	}
	result := make([]int, m.width*other.height)
	for w := 0; w < m.width; w++ {
		for h := 0; h < other.height; h++{
			summ := 0
			for n := 0; n < other.width; n++{
				summ+=m.Get(h, n)*other.Get(n, w);
			}
			result[h*m.width+w] = summ
		}
	}
	return CreateMatrixWithValue(m.width, other.height, result)
}