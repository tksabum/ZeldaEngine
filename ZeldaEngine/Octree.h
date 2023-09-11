#pragma once

#include "Vector3D.h"

#include <vector>
#include <unordered_map>

namespace ZeldaEngine
{
	template<typename DataType>
	class Octree
	{
		// Struct
	private:
		struct DataSet
		{
			ZeldaMath::Vector3D point;
			float radius;
			DataType* dataPtr = nullptr;
		};

		struct OctreeNode
		{
			ZeldaMath::Vector3D minPoint;
			ZeldaMath::Vector3D maxPoint;

			float maxRadius;
			OctreeNode* parentNode;
			std::vector<DataSet> points;
			OctreeNode* children[8];

			OctreeNode(ZeldaMath::Vector3D minpoint, ZeldaMath::Vector3D maxpoint, OctreeNode* parent = nullptr)
				: minPoint(minpoint), maxPoint(maxpoint), maxRadius(0.0f), parentNode(parent)
			{
				for (int i = 0; i < 8; i++)
				{
					children[i] = nullptr;
				}
			}

			// ���� ����Ʈ������ �����Ͱ� ������� ���� ���� ���� ����� ���� �ʴ� ������ �����Ͽ� points�� ��� ���� ��� �������� ������� ����
			bool IsLeafNode() const {
				return points.size() > 0;
			}
		};

	public:
		Octree(ZeldaMath::Vector3D minpoint, ZeldaMath::Vector3D maxpoint, ZeldaMath::Vector3D minareasize = ZeldaMath::Vector3D::zero)
			: root(new OctreeNode(minpoint, maxpoint)), rootMinPoint(minpoint), rootMaxPoint(maxpoint), minAreaSize(minareasize), autoRadiusUpdate(true)
		{

		}

		~Octree()
		{
			DestroyRecursive(root);
		}

		// ������ �߰�
		void Insert(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr)
		{
			InsertRecursive(center, radius, dataPtr, root);
		}

		// ������ ���� (Ȯ���ϰ� �����͸� ������ �� �ֵ��� center�� radius�� ������� �����ϴ� �Լ��� ����ų� �������� ���θ� ��ȯ�ϵ��� �����ϸ� ������...)
		void Remove(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr)
		{
			RemoveRecursive(center, radius, dataPtr, root);
		}

		void Remove(DataType* dataPtr)
		{
			assert(nodeTable.count(dataPtr) == 1); // ���� �����͸� �����Ϸ� ��
			OctreeNode* node = nodeTable[dataPtr];

			for (size_t i = 0; i < node->points.size(); i++)
			{
				if (node->points[i].dataPtr == dataPtr)
				{
					node->points.erase(node->points.begin() + i);
					assert(nodeTable.count(dataPtr) != 0);
					nodeTable.erase(dataPtr);
					if (autoRadiusUpdate)
					{
						UpdateRadiusRecursive(node);
					}

					ClearLeafNode(node);
					return;
				}
			}

			// ��ã������ ������ �ִ� ����
			assert(0);
			return;
		}

		// �̵�
		void Move(const ZeldaMath::Vector3D& position, float radius, DataType* dataPtr)
		{
			assert(nodeTable.count(dataPtr) == 1); // ���� �����͸� �̵��Ϸ� ��
			OctreeNode* node = nodeTable[dataPtr];

			bool isIntersects = Intersects(position, position, node->minPoint, node->maxPoint);

			if (isIntersects)
			{
				// ���� ����� �ٷ� �̵�
				for (size_t i = 0; i < node->points.size(); i++)
				{
					if (node->points[i].dataPtr == dataPtr)
					{
						node->points[i].point = position;
						node->points[i].radius = radius;

						// �̵� ����
						return;
					}
				}

				// ��ã������ ������ �ִ� ����
				assert(0);
				return;
			}
			else
			{
				// Insert�� assert(nodeTable.count(dataPtr) == 0)���� �ߴܵǴ� ��찡 ���� �߻��Ѵ�.
				// Remove���� �����͸� �������� ���� ���� ����Ǵµ� Ȯ���� �ʿ��ϴ�... -> dataPtr�� �����ϴ°ŷ� �����ϰ� �� �Ƹ� ��������?

				Remove(dataPtr);
				Insert(position, radius, dataPtr);
			}
		}

		// Ư�� �����ȿ� ���ԵǴ� �������� ����� �����´�.
		std::vector<DataType*> Query(const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			std::vector<DataType*> result;
			QueryRecursive(minPoint, maxPoint, root, result);
			return result;
		}

		void UpdateAllRadius()
		{
			UpdateAllRadiusRecursive(root);
		}

		void SetAutoRadiusUpdate(bool value)
		{
			autoRadiusUpdate = value;
		}

	private:
		OctreeNode* root;
		ZeldaMath::Vector3D rootMinPoint;
		ZeldaMath::Vector3D rootMaxPoint;
		ZeldaMath::Vector3D minAreaSize;

		std::unordered_map<DataType*, OctreeNode*> nodeTable;

		// ����, �����ÿ� �ڵ����� ������ �ִ밪�� ������Ʈ�Ѵ�.
		bool autoRadiusUpdate;


		// ��� node�� NULL�� �� �ִٴ� ������ ���µ� ���߿� �ѹ� Ȯ���� �� ��
		void DestroyRecursive(OctreeNode* node)
		{
			assert(node != nullptr);
			if (node != nullptr) return;

			// root ��尡 �ƴѰ�� parentNode�� ���󰡼� children���� �ڽ��� ����
			if (node->parentNode != nullptr)
			{
				for (size_t i = 0; i < 8; i++)
				{
					if (node->parentNode->children[i] == node)
					{
						node->parentNode->children[i] = nullptr;
					}
				}
			}
			// root ����ΰ�� root = nullptr
			else
			{
				root = nullptr;
			}

			// nullptr�� �ƴ� ��� �ڽ� ��忡 ���ؼ� DestroyRecursive
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					DestroyRecursive(node->children[i]);
				}
			}

			delete node;
		}
		
		// �ڽĳ���� ��ȣ ���
		int GetOctant(const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			int octant = 0;
			ZeldaMath::Vector3D midPoint = (minPoint + maxPoint) * 0.5f;

			if ((maxPoint.x - minPoint.x) * 0.5f >= minAreaSize.x && point.x > midPoint.x)
				octant |= 1;
			if ((maxPoint.y - minPoint.y) * 0.5f >= minAreaSize.y && point.y > midPoint.y)
				octant |= 2;
			if ((maxPoint.z - minPoint.z) * 0.5f >= minAreaSize.z && point.z > midPoint.z)
				octant |= 4;

			return octant;
		}

		ZeldaMath::Vector3D GetOctantMinPoint(int octant, const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			bool holdx = (maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x;
			bool holdy = (maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y;
			bool holdz = (maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z;

			ZeldaMath::Vector3D octantMinPoint = ZeldaMath::Vector3D(
				(octant & 1) ? (minPoint.x + maxPoint.x) * 0.5f : minPoint.x,
				(octant & 2) ? (minPoint.y + maxPoint.y) * 0.5f : minPoint.y,
				(octant & 4) ? (minPoint.z + maxPoint.z) * 0.5f : minPoint.z
			);

			if (holdx) octantMinPoint.x = minPoint.x;
			if (holdy) octantMinPoint.y = minPoint.y;
			if (holdz) octantMinPoint.z = minPoint.z;

			return octantMinPoint;
		}

		ZeldaMath::Vector3D GetOctantMaxPoint(int octant, const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			bool holdx = (maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x;
			bool holdy = (maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y;
			bool holdz = (maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z;

			ZeldaMath::Vector3D octantMaxPoint = ZeldaMath::Vector3D(
				(octant & 1) ? maxPoint.x : (minPoint.x + maxPoint.x) * 0.5f,
				(octant & 2) ? maxPoint.y : (minPoint.y + maxPoint.y) * 0.5f,
				(octant & 4) ? maxPoint.z : (minPoint.z + maxPoint.z) * 0.5f
			);

			if (holdx) octantMaxPoint.x = maxPoint.x;
			if (holdy) octantMaxPoint.y = maxPoint.y;
			if (holdz) octantMaxPoint.z = maxPoint.z;

			return octantMaxPoint;
		}

		void InsertRecursive(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr, OctreeNode* node)
		{
			const ZeldaMath::Vector3D& minPoint = node->minPoint;
			const ZeldaMath::Vector3D& maxPoint = node->maxPoint;

			// center�� ���� �ۿ� �ִ� ��� ����
			if (!Intersects(center, center, minPoint, maxPoint))
				return;

			// LeafNode�� ������ ���� �� ����
			if (node->IsLeafNode())
			{
				node->points.push_back({ center, radius, dataPtr });
				if (autoRadiusUpdate)
				{
					UpdateRadiusRecursive(node);
				}
				assert(nodeTable.count(dataPtr) == 0);
				nodeTable[dataPtr] = node;
				return;
			}

			// �� �̻� ���� �ϸ� �ּ� ũ�⺸�� �۾����� ��� ������ ���� �� ����
			if (((maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x) && ((maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y) && ((maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z))
			{
				node->points.push_back({ center, radius, dataPtr });
				if (autoRadiusUpdate)
				{
					UpdateRadiusRecursive(node);
				}
				assert(nodeTable.count(dataPtr) == 0);
				nodeTable[dataPtr] = node;
				return;
			}

			int octant = GetOctant(center, minPoint, maxPoint);

			ZeldaMath::Vector3D childMinPoint = GetOctantMinPoint(octant, center, minPoint, maxPoint);
			ZeldaMath::Vector3D childMaxPoint = GetOctantMaxPoint(octant, center, minPoint, maxPoint);

			// ã�� �ִ� ocatant�� ���ٸ� ���ο� �ڽ� ��带 �����.
			if (node->children[octant] == nullptr)
			{
				node->children[octant] = new OctreeNode(childMinPoint, childMaxPoint, node);
			}

			// ���ȣ��� �ڽ� ��忡 �����͸� �����Ѵ�.
			InsertRecursive(center, radius, dataPtr, node->children[octant]);
		}
		
		void UpdateRadiusRecursive(OctreeNode* node)
		{
			if (node == nullptr)
			{
				return;
			}

			float result = 0.0f;

			// ��������� ��� �������� �������� Ȯ���Ͽ� �ִ��� ���Ѵ�.
			if (node->IsLeafNode())
			{
				for (size_t i = 0; i < node->points.size(); i++)
				{
					float currentradius = node->points[i].radius;
					if (result < currentradius)
					{
						result = currentradius;
					}
				}
			}
			// ������尡 �ƴ϶�� ��� �ڽ� ��忡�� �ִ� Ȯ��
			else
			{
				for (size_t i = 0; i < 8; i++)
				{
					float currentradius = 0.0f;
					if (node->children[i] == nullptr) continue;

					currentradius = node->children[i]->maxRadius;

					if (result < currentradius)
					{
						result = currentradius;
					}
				}
			}

			float prevMax = node->maxRadius;
			node->maxRadius = result;

			// maxRadius�� ����ƴٸ� parentNode�� ������Ʈ �Ѵ�.
			if (prevMax != result)
			{
				UpdateRadiusRecursive(node->parentNode);
			}
		}

		// �ִ� ������ ����
		float UpdateAllRadiusRecursive(OctreeNode* node)
		{
			float result = 0.0f;

			// ��������� ��� �������� �������� Ȯ���Ͽ� �ִ��� node�� �����ϰ� ����
			if (node->IsLeafNode())
			{
				for (size_t i = 0; i < node->points.size(); i++)
				{
					float currentradius = node->points[i].radius;
					if (result < currentradius)
					{
						result = currentradius;
					}
				}

				node->maxRadius = result;

				return result;
			}

			// ��� �ڽ� ����� ������ �ִ��� ã�´�.
			for (size_t i = 0; i < 8; i++)
			{
				if (node->children[i] == nullptr)
				{
					continue;
				}

				float currentradius = UpdateAllRadiusRecursive(node->children[i]);
				if (result < currentradius)
				{
					result = currentradius;
				}
			}

			node->maxRadius = result;

			return result;
		}

		void RemoveRecursive(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr, OctreeNode* node)
		{
			const ZeldaMath::Vector3D& minPoint = node->minPoint;
			const ZeldaMath::Vector3D& maxPoint = node->maxPoint;

			// center�� ���� ���� �����̸� ����
			if (!Intersects(center, center, minPoint, maxPoint))
				return;

			// ��������� dataPtr�� ���Ͽ� �����͸� ã�� ���� �� �����Ѵ�.
			if (node->IsLeafNode())
			{
				for (int i = 0; i < node->points.size(); i++)
				{
					if (node->points[i].dataPtr == dataPtr)
					{
						node->points.erase(node->points.begin() + i);
						assert(nodeTable.count(dataPtr) != 0);
						nodeTable.erase(dataPtr);
						if (autoRadiusUpdate)
						{
							UpdateRadiusRecursive(node);
						}
						break;
					}
				}

				ClearLeafNode(node);
				return;
			}

			int octant = GetOctant(center, minPoint, maxPoint);

			// �ڽ� ��忡�� �ٽ� Ž��
			if (node->children[octant] != nullptr)
			{
				RemoveRecursive(center, radius, dataPtr, node->children[octant]);
			}
		}

		// �����Ͱ� �����Ǿ� ������尡 �ƴϰ� �� ��带 �����ϱ� ���� �Լ�
		void ClearLeafNode(OctreeNode* node)
		{
			// ��������� ���� ����
			if (node->IsLeafNode())
			{
				return;
			}

			// �ڽ� ����� ������ Ȯ���Ѵ�
			int childCount = 0;
			for (size_t i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					childCount++;
				}
			}

			// �ڽĳ�尡 ���ٸ� �����Ͱ� ������ ��������̹Ƿ� �����Ѵ�. ���� �� �� �θ� ������Ʈ�� Ȯ���ؾ� �Ѵ�.
			if (childCount == 0)
			{
				OctreeNode* parent = node->parentNode;
				DestroyRecursive(node);
				ClearLeafNode(parent);
			}
		}

		void QueryRecursive(const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint, OctreeNode* node, std::vector<DataType*>& result) const
		{
			// node�� ������ node���� ������ �ִ� ������ ũ�⸸ŭ Ȯ��� ������ Ž�� ������ ��ġ�� �ʴ´ٸ� ����
			if (!Intersects(minPoint, maxPoint, node->minPoint - ZeldaMath::Vector3D(node->maxRadius, node->maxRadius, node->maxRadius), node->maxPoint + ZeldaMath::Vector3D(node->maxRadius, node->maxRadius, node->maxRadius)))
				return;

			// LeafNode��� ������ ������ ���������� ���� �� ��Ȯ�� ������ �Ͽ� ����� �߰�
			if (node->IsLeafNode())
			{
				for (const DataSet& data : node->points)
				{
					if (Intersects(minPoint, maxPoint, data.point - ZeldaMath::Vector3D(data.radius, data.radius, data.radius), data.point + ZeldaMath::Vector3D(data.radius, data.radius, data.radius)))
					{
						result.push_back(data.dataPtr);
					}
				}
				return;
			}

			// �� ���� ��쿡 ��� �ڽ� ���鿡 ���ؼ� ���ȣ��
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					QueryRecursive(minPoint, maxPoint, node->children[i], result);
				}
			}
		}
		
		// point1�� �̷�� ������ point2�� �̷�� ������ ��ġ���� Ȯ�� (��ġ�� ������ �ִٸ� true)
		bool Intersects(ZeldaMath::Vector3D minPoint1, ZeldaMath::Vector3D maxPoint1, ZeldaMath::Vector3D minPoint2, ZeldaMath::Vector3D maxPoint2) const
		{
			if (maxPoint1.x < rootMinPoint.x) maxPoint1.x = rootMinPoint.x;
			if (maxPoint1.y < rootMinPoint.y) maxPoint1.y = rootMinPoint.y;
			if (maxPoint1.z < rootMinPoint.z) maxPoint1.z = rootMinPoint.z;

			if (rootMaxPoint.x < minPoint1.x) minPoint1.x = rootMaxPoint.x;
			if (rootMaxPoint.y < minPoint1.y) minPoint1.y = rootMaxPoint.y;
			if (rootMaxPoint.z < minPoint1.z) minPoint1.z = rootMaxPoint.z;

			if (maxPoint2.x < rootMinPoint.x) maxPoint2.x = rootMinPoint.x;
			if (maxPoint2.y < rootMinPoint.y) maxPoint2.y = rootMinPoint.y;
			if (maxPoint2.z < rootMinPoint.z) maxPoint2.z = rootMinPoint.z;

			if (rootMaxPoint.x < minPoint2.x) minPoint2.x = rootMaxPoint.x;
			if (rootMaxPoint.y < minPoint2.y) minPoint2.y = rootMaxPoint.y;
			if (rootMaxPoint.z < minPoint2.z) minPoint2.z = rootMaxPoint.z;

			if (maxPoint1.x < minPoint2.x || minPoint1.x > maxPoint2.x)
				return false;
			if (maxPoint1.y < minPoint2.y || minPoint1.y > maxPoint2.y)
				return false;
			if (maxPoint1.z < minPoint2.z || minPoint1.z > maxPoint2.z)
				return false;
			return true;
		}
	};
}