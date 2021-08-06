package ce2d_collision

type QuadTree struct {
	Parent  *QuadTree
	Deep    int
	Rects   [4]*QuadTree
	Colider []AABB
}

func CreateQuadTree(width, height int) *QuadTree {
	return &QuadTree{}
}

func CreateEmptyQuadTree(width, height int) *QuadTree {
	return &QuadTree{}
}
