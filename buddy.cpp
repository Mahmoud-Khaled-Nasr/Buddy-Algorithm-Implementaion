#include "buddy.h"
int buddy::MEMORY_SIZE = 1024;
buddy::buddy() {
	buildlog();
}
void buddy::build(int l, int r, int idx, int val) {
	if (l == r) {
		tree[idx].first = val;
		tree[idx].second = val;
		return;
	}
	int mid = (l + r) >> 1;
	int tempidx1 = (idx << 1); int tempidx2 = tempidx1 | 1;
	build(l, mid, tempidx1, val / 2);
	build(mid + 1, r, tempidx2, val / 2);
	tree[idx].first = tree[tempidx1].first + tree[tempidx2].first;
	tree[idx].second = tree[tempidx1].second + tree[tempidx2].second;
}
pair<int, int> buddy::allocate(int size) {
	int a = 1;
	while (a < size)a *= 2;
	return getLocationInMemory(1, MEMORY_SIZE, 1, a);
}
pair<int, int> buddy::getLocationInMemory(int l, int r, int idx, int size)
{
	pair<int, int>p = { -1,-1 };

	int sizee = r - l + 1;
	bool flagleft = false; bool flagright = false;
	int i1, i2;
	for (i1 = nearstlog[size]; (1 << i1) <= sizee; ++i1) { if ((tree[idx].first & (1 << i1))) { flagleft = true; break; } }
	for (i2 = nearstlog[size]; (1 << i2) <= sizee; ++i2) { if ((tree[idx].second & (1 << i2))) { flagright = true; break; } }
	if (!flagleft && !flagright)return p;
	if (sizee == size) {
		tree[idx].first = 0; tree[idx].second = 0;
		return{ l,r };
	}
	int mid = (l + r) >> 1;
	int tempidx1 = (idx << 1); int tempidx2 = tempidx1 | 1;
	bool leftt = true; bool rightt = true;
	if (flagleft&&flagright) { // deh bshof beha ely 22rb mt2sm lya b7ot el tany b false
		if (i1 <= i2)rightt = false;
		else leftt = false;
	}
	if (flagleft&&leftt) {
		tree[idx].first = 0; tree[idx].second = 0;
		p = getLocationInMemory(l, mid, tempidx1, size);
		tree[idx].first |= (tree[tempidx1].first | tree[tempidx1].second);
		tree[idx].second |= (tree[tempidx2].first | tree[tempidx2].second);
		return p;
	}

	if (flagright&&rightt) {
		tree[idx].first = 0; tree[idx].second = 0;
		p = getLocationInMemory(mid + 1, r, tempidx2, size);
		tree[idx].first |= (tree[tempidx1].first | tree[tempidx1].second);
		tree[idx].second |= (tree[tempidx2].first | tree[tempidx2].second);
		return p;
	}
}
void buddy::removeFromMemory(int l, int r, int idx, int x, int y, int val)
{
	if (l > y || r < x)return;
	if (l == x&&r == y) {
		int size = r - l;
		tree[idx].first |= val;
		tree[idx].second |= val;
		return;
	}
	int mid = (l + r) >> 1;
	int tempidx1 = (idx << 1); int tempidx2 = tempidx1 | 1;
	if (l <= x&&mid >= y)
	{
		tree[idx].first = 0;
		removeFromMemory(l, mid, tempidx1, x, y, val / 2);
		tree[idx].first |= (tree[tempidx1].first | tree[tempidx1].second);
	}
	if ((mid + 1) <= x&&r >= y) {
		tree[idx].second = 0;
		removeFromMemory(mid + 1, r, tempidx2, x, y, val / 2);
		tree[idx].second |= (tree[tempidx2].first | tree[tempidx2].second);
	}
	if (tree[tempidx1].first == (val / 2) && tree[tempidx2].first == (val / 2)) {
		tree[idx].first = 0; tree[idx].second = 0;
		tree[idx].first |= val;
		tree[idx].second |= val;
	}
}
void buddy::buildlog() {
	nearstlog[0] = -1;
	for (int i = 1; i <= 2000; ++i)nearstlog[i] = nearstlog[i - 1] + !(i&(i - 1));
}


