#include "W4Logger.h"


namespace w4::pathfinding {

template<typename Field>
AStarSolver<Field>::AStarSolver(const Field& field)
        : m_field(field)
{
}

template<typename Field>
void AStarSolver<Field>::solve(const CellType& startCell)
{
    m_startCell = startCell;
    m_cellsDestination.clear();

    std::deque<std::pair<CellType, int>> m_searchPoints;

    m_searchPoints.emplace_back(startCell, 0);

    int currentLength = 0;
    auto searchFunc = [this, &m_searchPoints, &currentLength](const CellType &cell)
    {
        auto it = m_cellsDestination.find(cell);
        if (it == m_cellsDestination.end())
        {
            if (m_field.isEmpty(cell))
            {
                m_searchPoints.emplace_back(cell, currentLength);
                m_cellsDestination[cell] = 0;
            }
        }
    };

    while (!m_searchPoints.empty())
    {
        auto currentPoint = m_searchPoints.front();
        m_searchPoints.pop_front();

        auto it = m_cellsDestination.find(currentPoint.first);
        if (it != m_cellsDestination.end())
        {
            if (it->second > 0)
            {
                continue;
            }
        }

        currentLength = currentPoint.second + 1;
        m_cellsDestination[currentPoint.first] = currentLength;

        if (currentLength > m_furtherLength)
        {
            m_furtherLength = currentLength;
            m_furtherPoint = {currentPoint.first};
        }

        m_field.iterateNeighbors(currentPoint.first, searchFunc);
    }
}

template<typename Field>
bool AStarSolver<Field>::isCellReachable(const CellType& cell) const
{
    auto it = m_cellsDestination.find(cell);
    return it != m_cellsDestination.end();
}

template<typename Field>
const typename AStarSolver<Field>::CellType& AStarSolver<Field>::getFurtherPoint() const
{
    return m_furtherPoint;
}

template<typename Field>
std::deque<typename AStarSolver<Field>::CellType> AStarSolver<Field>::constructPathToCell(const CellType& destination) const
{
    std::deque<CellType> res;
    if (!isCellReachable(destination))
    {
        return res;
    }

    auto currPoint = destination;
    auto currLength = m_cellsDestination.find(destination)->second;

    auto searchFunc = [this, &currPoint, &currLength](const CellType &cell)
    {
        auto it = m_cellsDestination.find(cell);
        if (it == m_cellsDestination.end())
        {
            return;
        }
        auto cellPathLength = it->second;

        if (currLength > cellPathLength)
        {
            currPoint = cell;
            currLength = cellPathLength;
        }
    };

    do
    {
        m_field.iterateNeighbors(currPoint, searchFunc);
        res.push_back(currPoint);
    } while (currPoint != m_startCell);

    return res;
}

template<typename Field>
AStar<Field>::AStar(Field & field)
    : m_field(field)
{

}

template<typename Field>
typename AStar<Field>::Path AStar<Field>::getPath(const Cell& from, const Cell& to) const
{
    class NeighboursCache: public NeighboursCollector
    {
        std::vector<Cell> m_neighbours;

    public:
        NeighboursCache()
        {
            m_neighbours.reserve(8);
        }

        virtual void push(const Cell& cell) override
        {
            m_neighbours.emplace_back(cell);
        }

        const Cell * begin() const
        {
            return m_neighbours.data();
        }

        const Cell * end() const
        {
            return m_neighbours.data() + m_neighbours.size();
        }

        void clear()
        {
            m_neighbours.clear();
        }
    } static neighboursCache;

    struct Node
    {
        Cell point;
        const Node * parent;
        float costFromStart;
        float estimatedCostToEnd;
        float cost;

        Node(const Cell& ptr, const Node * prnt, float c, float e)
                : point(ptr)
                  , parent(prnt)
                  , costFromStart(c)
                  , estimatedCostToEnd(e)
                  , cost(c + e)
        {}

        bool operator==(const Node& rh) const
        {
            return point == rh.point;
        }
    };
    struct NodeLessCost
    {
        bool operator()(const Node& lh, const Node& rh) const
        {
            return lh.cost < rh.cost;
        }
        bool operator()(const Node* lh, const Node* rh) const
        {
            return lh->cost < rh->cost;
        }
    };

    struct NodeHash
    {
        size_t operator()(const Node& node) const
        {
            return CellHash()(node.point);
        }
    };

    std::unordered_set<Node, NodeHash> allNodes;
    std::multiset<const Node*, NodeLessCost> openList;

    openList.emplace(&*allNodes.emplace(from, nullptr, 0.f, 0.f).first);

    auto generatePath = [&](const Node& node) -> Path
    {
        Path result;
        auto ptr = &node;
        do
        {
            result.push_back(ptr->point);
            ptr = ptr->parent;
        } while (ptr);

        std::reverse(result.begin(), result.end());
        return result;
    };

    while (!openList.empty())
    {
        const Node * processedNode = *openList.begin();
        openList.erase(openList.begin());

        neighboursCache.clear();
        m_field.getNeighbours(processedNode->point, neighboursCache);
        for (auto & n: neighboursCache)
        {
            Node node(n, processedNode, m_field.getDistance(processedNode->point, n) + processedNode->costFromStart, m_field.getDistance(n, to));
            if (node.point == to)
            {
                return generatePath(node);
            }
            auto it = allNodes.find(node);
            if (it != allNodes.end())
            {
                if ((it->cost - node.cost) < w4::math::EPSILON)
                {
                    continue;
                }
                openList.erase(&(*it));
                allNodes.erase(it);
            }
            openList.emplace(&*allNodes.emplace(node).first);
        }
    }
    //path not found
    return Path{};
}

}