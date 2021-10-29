package ECS

import (
	"sync/atomic"

	"github.com/Falldot/ce2d/pkg/Math/Container"
)

// Установка максимального кол-во "Компонентов" в "Сущности", позволит
// использовать массив, в замен менее производительной "Хэш Таблицы" (map)
const maxLenghtComponentInEntity = 32

var (
	// Глобальный счетчик для id "Сущностей"
	idIncEntity uint64

	// Глобальный контейнер для хранения активных "Сущностей"
	entities map[uint64]Entity

	// Кэш "Сущностей", представленный более производительным односвязным списком
	entitiesCache       Container.List
	lenghtEntitiesCashe int

	// Глобальный контейнер для удаленный сущностей
	/*
		"Пул Сущностей" позволит нам не создавать новые сущности, а
		переиспользовать удаленные, что даст немного производительности
		в момент создания новых объектов, при условии что мы удаляли их ранее
	*/
	entitiesPool       Container.DList
	lenghtEntitiesPool int
)

///////////////////////////////////////////////////////////
// PUBLIC методы
///////////////////////////////////////////////////////////

// Инициализация ECS
func Init() {
	idIncEntity = 0
	entities = make(map[uint64]Entity)
	entitiesCache = Container.NewList()
	entitiesPool = Container.NewDList()
}

// Создать "Сущность"
func CreateEntity() Entity {
	// Получаем экземпляр Сущности из пула сущностей
	e := getEntity()
	// Записываем сущности в хэш таблицу "Сущностей", использюя id как ключ
	entities[e.ID()] = e

	// Записываем сущность в кэш, если он не пуст
	if entitiesCache.Empty() {
		return e
	}

	entitiesCache.Add(e)
	lenghtEntitiesCashe++

	// for _, g := range p.groups {
	// 	g.handleEntity(e)
	// }

	return e
}

// Удалить все "Сущности"
func DestroyAllEntities() {
	entities = make(map[uint64]Entity)
	entitiesCache.Clear()
	lenghtEntitiesCashe = 0
}

///////////////////////////////////////////////////////////
// Lenght методы
///////////////////////////////////////////////////////////
func LenghtEntities() int {
	return len(entities)
}

func LenghtEntitiesPool() int {
	return lenghtEntitiesPool
}

func LenghtEntitiesCashe() int {
	return lenghtEntitiesCashe
}

///////////////////////////////////////////////////////////
// PRIVATE методы
///////////////////////////////////////////////////////////

// Получить "Сущность"
func getEntity() Entity {
	// Объявляем
	var entity Entity

	// Проверяем, пуст ли он?
	if lenghtEntitiesPool > 0 {
		// Если нет, то переиспользуем его, перенося из удаленных в активных
		ellist := entitiesPool.First()
		entity = ellist.Value().(Entity)
		ellist.Remove()
		lenghtEntitiesPool--
	} else {
		// Иначе создаем новый объект
		entity = createEntity(idIncEntity)
		// Пакет atomic делает генерацию id потокобезопасной
		atomic.AddUint64(&idIncEntity, 1)
	}

	return entity
}

// Получить все "Сущности" (кэшируеться)
func getEntities() Container.List {
	if !entitiesCache.Empty() {
		return entitiesCache
	}
	for _, e := range entities {
		entitiesCache.Add(e)
		lenghtEntitiesCashe++
	}
	return entitiesCache
}

// Удалить "Сущность"
func destroyEntity(e Entity) {
	// Удаляем все компоненты
	e.removeAllComponents()

	// Удаляем из глобальной хеш таблицы
	delete(entities, e.ID())

	// for _, g := range p.groups {
	// 	g.handleEntity(e)
	// }

	// Обнуляем кэш
	entitiesCache.Clear()
	lenghtEntitiesCashe = 0

	// Возвращаем в пул
	entitiesPool.Add(e)
	lenghtEntitiesPool++
}
