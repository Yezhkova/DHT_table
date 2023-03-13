#include "BucketMap.h"
#include "Node.h"
#include "Utils.h"

int BucketArray::g_bucketSize = 20; // maximum amount of nodes in a bucket
//const int g_bucketMultiplier = 8; // optimization for 160 buckets in map
int BucketArray::g_treeSize = 160; // maximum amount of buckets in a map

const std::array<Bucket, 160>& BucketArray::data() const {
	return m_Buckets;
};

int BucketArray::calcBucketIndex(const ID& id) {
	//return (m_node.id().distance(id) - 1) / g_bucketMultiplier;
	/*if (m_node.id().distance(id) - 1 == 160) {
		LOG("debug");
	}*/
	//LOG(m_node.id() << " dist " << id << " = " << std::dec << m_node.id().distance(id));
	return id == m_node.id() ? 0 : m_node.id().distance(id) - 1;
}

const Bucket& BucketArray::getBucket(int index) const {
	return m_Buckets[index];
}

const size_t BucketArray::bucketSize(int bucketIdx) const {
	return m_Buckets[bucketIdx].size();
}

const bool BucketArray::bucketFull(int bucketIdx) const {
	return bucketSize(bucketIdx) == BucketArray::g_bucketSize;
}

bool BucketArray::addNode(const ID& id, int BucketIndex) {
	return m_Buckets[BucketIndex].insert(Contact{ id }).second;
}

bool BucketArray::removeNode(const ID& id) {
	size_t BucketIndex = calcBucketIndex(id);
	return m_Buckets[BucketIndex].erase(Contact{ id });
}

bool BucketArray::containsNode(const ID& id) const
{
	for (auto& bucket : m_Buckets)
	{
		if (bucket.find(id) != bucket.end()) {
			return true;
		}
	}
	return false;
}

const size_t BucketArray::size() const {
	return m_Buckets.size();
}

std::ostream& operator<< (std::ostream& out, const BucketArray& b)
{
	out << b.m_node.id() << " bucket map size: " << std::dec << b.m_Buckets.size() << '\n';
	for (int i = 0; i < BucketArray::g_treeSize; ++i) {
		if (!b.data()[i].empty()) {
			out << "depth " << std::dec << i << '\n';
			for (auto& conact : b.data()[i]) {
				out << conact.id() << "\n";
			}
		}
	}
	return out;
}
