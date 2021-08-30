#pragma once

#include "W4Math.h"

#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace w4::pathfinding {

template<typename Field>
class AStarSolver {
public:
    using CellType = typename Field::CellType;

    AStarSolver(const Field &field);
    void solve(const CellType &startCell);
    bool isCellReachable(const CellType &cell) const;
    const CellType &getFurtherPoint() const;
    std::deque <CellType> constructPathToCell(const CellType &destination) const ;
private:
    const Field &m_field;
    CellType m_startCell;

    std::unordered_map<CellType, int> m_cellsDestination;

    CellType m_furtherPoint;
    int m_furtherLength = 0;
};

/*
 * AStar - class realize AStar algorithm on Field
 * Field contract:
 *      - must have typename AStarCellType - cell type
 *      - must have typename AStarCellTypeHash - hasher with operator 'size_t operator()(const AStarCellType&) const';
 *      - must have method getNeighbours with signature 'void getNeighbours(const AStarCellType& cell, AStar<Field>::NeighboursCollector & neighboursCollector) const'
 *          all passable neighbours of cell must be added to neighboursCollector
 *      - must have method getDistance with signature 'float getDistance(const AStarCellType& from, const AStarCellType& to) const'
 *          return expected distance between 'from' and 'to', usually just (from - to).length()
 * */
template<typename Field>
class AStar
{
public:
    using Cell = typename Field::AStarCellType;
    using CellHash = typename Field::AStarCellTypeHash;
    using Path = std::vector<Cell>;
    struct NeighboursCollector
    {
        virtual void push(const Cell&) = 0;
    };

public:
    AStar(Field & field);

    Path getPath(const Cell& from, const Cell& to) const;

private:
    Field & m_field;
};

/*
 * 2D field helper for AStar algorithm
 * */
class AStar2DField
{
public:
    using AStarCellType = w4::math::ivec2;
    struct AStarCellTypeHash
    {
        size_t operator()(const AStarCellType& cell) const;
    };

public:
    explicit AStar2DField(AStarCellType::value_type maxX, AStarCellType::value_type maxY);
    AStar2DField(const AStar2DField&);
    AStar2DField(AStar2DField&&);
    virtual ~AStar2DField();

    AStar2DField & operator=(const AStar2DField&);
    AStar2DField & operator=(AStar2DField&&);

    void resizeField(AStarCellType::value_type maxX, AStarCellType::value_type maxY);
    AStar<AStar2DField>::Path getPath(const AStarCellType& from, const AStarCellType& to) const;

private:
    friend class AStar<AStar2DField>;
    float getDistance(const AStarCellType& from, const AStarCellType& to) const;

protected:
    virtual void getNeighbours(const AStarCellType& cell, AStar<AStar2DField>::NeighboursCollector & neighboursCollector) const = 0;

private:
    AStarCellType::value_type m_maxX;
    AStarCellType::value_type m_maxY;
    AStar<AStar2DField> m_aStar;
};

} //namespace w4::pathfinding

#include "impl/AStar.inl"