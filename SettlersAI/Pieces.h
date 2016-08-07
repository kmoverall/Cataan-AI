#pragma once

#include "stdafx.h"


enum class CornerBuilding { None, Settlement, City };
enum class EdgeBuilding { None, Road };
enum class Resource { None, Brick, Lumber, Ore, Grain, Wool };
enum class Terrain { Desert, Hills, Forest, Mountains, Fields, Pasture };
enum class Port { None, Brick, Lumber, Ore, Grain, Wool, Any };
enum class DevelopmentCard { Knight, RoadBuilding, YearOfPlenty, Monopoly, VictoryPoint};

struct HexCoord {
	int p, q, r;

	HexCoord() : p(0), q(0), r(0) {}

	HexCoord(int p_, int q_, int r_) : p(p_), q(q_), r(r_) {
		assert(p + q + r == 0);
	}

	static int Length(HexCoord hex) {
		return int((abs(hex.p) + abs(hex.q) + abs(hex.r)) / 2);
	}

	static int Distance(HexCoord a, HexCoord b) {
		return Length(a - b);
	}

	HexCoord& operator+=(const HexCoord& rhs) {
		p += rhs.p;
		q += rhs.q;
		r += rhs.r;
		return *this;
	}
	friend HexCoord operator+(HexCoord lhs, const HexCoord& rhs) {
		lhs += rhs;
		return lhs;
	}

	HexCoord& operator-=(const HexCoord& rhs) {
		p -= rhs.p;
		q -= rhs.q;
		r -= rhs.r;
		return *this;
	}
	friend HexCoord operator-(HexCoord lhs, const HexCoord& rhs) {
		lhs -= rhs;
		return lhs;
	}

	HexCoord& operator*=(const int rhs) {
		p *= rhs;
		q *= rhs;
		r *= rhs;
		return *this;
	}
	friend HexCoord operator*(HexCoord lhs, const int rhs) {
		lhs *= rhs;
		return lhs;
	}

	friend bool operator==(const HexCoord& lhs, const HexCoord& rhs) {
		return lhs.p == rhs.p && lhs.q == rhs.q && lhs.r == rhs.r;
	}
	friend bool operator!=(const HexCoord& lhs, const HexCoord& rhs) {
		return !(lhs == rhs);
	}
};

namespace std {
	template <> struct hash<HexCoord> {
		size_t operator()(const HexCoord& h) const {
			hash<int> int_hash;
			size_t hp = int_hash(h.p);
			size_t hq = int_hash(h.q);
			return hp ^ (hq + 0x9e3779b9 + (hp << 6) + (hp >> 2));
		}
	};
}

struct Corner {
	int owner = -1;
	CornerBuilding building = CornerBuilding::None;
	const int index;

	Corner(int i_) : index(i_) {}
};

struct Edge {
	int owner = -1;
	EdgeBuilding building = EdgeBuilding::None;
	Port port = Port::None;
	const int index;

	Edge(int i_) : index(i_) {}
	Edge(int i_, Port port_) : index(i_), port(port_) {}
};

struct Hex {
	int value = 0;
	Terrain terrain = Terrain::Desert;

	std::shared_ptr<Corner> corners[6];
	std::shared_ptr<Edge> edges[6];

	Hex() {
		for (int i = 0; i < 6; i++) {
			corners[i] = nullptr;
			edges[i] = nullptr;
		}
	}

	Hex(int value_, Terrain terr_) : value(value_), terrain(terr_) {
		for (int i = 0; i < 6; i++) {
			corners[i] = nullptr;
			edges[i] = nullptr;
		}
	}
};

namespace HexDirections {
	static const HexCoord NORTH = HexCoord(0, 1, -1);
	static const HexCoord NORTHEAST = HexCoord(1, 0, -1);
	static const HexCoord SOUTHEAST = HexCoord(1, -1, 0);
	static const HexCoord SOUTH = HexCoord(0, -1, 1);
	static const HexCoord SOUTHWEST = HexCoord(-1, 0, 1);
	static const HexCoord NORTHWEST = HexCoord(-1, 1, 0);

	static const HexCoord VECTORS[6] = {NORTH, NORTHEAST, SOUTHEAST, SOUTH, SOUTHWEST, NORTHWEST};

	enum class Direction {North, Northeast, Southeast, South, Southwest, Northwest};
}
