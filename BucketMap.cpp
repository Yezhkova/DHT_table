#include "BucketMap.h"
#include "Node.h"
#include "Utils.h"
#include "Constants.h"

const std::array<Bucket, 160>& BucketArray::data() const {
	return m_Buckets;
};

size_t BucketArray::calcBucketIndex(const ID& id) const{
	return id == m_node.id() ? 0 : m_node.id().distance(id) - 1;
}

const Bucket& BucketArray::getBucket(int index) const {
	return m_Buckets[index];
}

const size_t BucketArray::bucketSize(int bucketIdx) const {
	return m_Buckets[bucketIdx].size();
}

const bool BucketArray::bucketFull(int bucketIdx) const {
	return bucketSize(bucketIdx) == BUCKET_SIZE;
}

bool BucketArray::addNode(const ID& id, int BucketIndex) {
	return m_Buckets[BucketIndex].insert(Contact{ id }).second;
}

bool BucketArray::removeNode(const ID& id) {
	size_t BucketIndex = calcBucketIndex(id);
	return m_Buckets[BucketIndex].erase(Contact{ id });
}

const Contact* BucketArray::getContactPtr(const ID& id) const
{
	size_t bucketIndex = calcBucketIndex(id);
	if (auto it = m_Buckets[bucketIndex].find(id); it != m_Buckets[bucketIndex].end()) {
		return &(*it);
	}
	return nullptr;
}

const size_t BucketArray::size() const {
	return m_Buckets.size();
}

std::ostream& operator<< (std::ostream& out, const BucketArray& b)
{
	out << b.m_node.id() << " bucket map size: " << std::dec << b.m_Buckets.size() << '\n';
	for (int i = 0; i < TREE_SIZE; ++i) {
		if (!b.data()[i].empty()) {
			out << "depth " << std::dec << i << '\n';
			for (auto& conact : b.data()[i]) {
				out << conact.id() << "\n";
			}
		}
	}
	return out;
}
