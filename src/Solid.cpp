// Copyright (c) 2012-2013, IGN France.
// Copyright (c) 2012-2022, Oslandia.
// SPDX-License-Identifier: GPL-2.0-or-later

#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/Solid.h>

namespace SFCGAL {

///
///
///
Solid::Solid() { _shells.push_back(new PolyhedralSurface()); }

///
///
///
Solid::Solid(const PolyhedralSurface &exteriorShell)
{
  _shells.push_back(exteriorShell.clone());
}

///
///
///
Solid::Solid(PolyhedralSurface *exteriorShell)
{
  _shells.push_back(exteriorShell);
}

///
///
///
Solid::Solid(const std::vector<PolyhedralSurface> &shells)
{
  if (shells.empty()) {
    _shells.resize(1, new PolyhedralSurface());
  } else {
    for (const auto &shell : shells) {
      _shells.push_back(shell.clone());
    }
  }
}

///
///
///
Solid::Solid(const Solid &other) : Geometry(other)
{
  for (size_t i = 0; i < other.numShells(); i++) {
    _shells.push_back(other.shellN(i).clone());
  }
}

///
///
///
auto
Solid::operator=(Solid other) -> Solid &
{
  swap(other);
  return *this;
}

///
///
///
Solid::~Solid() = default;

///
///
///
auto
Solid::clone() const -> Solid *
{
  return new Solid(*this);
}

///
///
///
auto
Solid::geometryType() const -> std::string
{
  return "Solid";
}

///
///
///
auto
Solid::geometryTypeId() const -> GeometryType
{
  return TYPE_SOLID;
}

///
///
///
auto
Solid::dimension() const -> int
{
  return 3;
}

///
///
///
auto
Solid::coordinateDimension() const -> int
{
  return exteriorShell().coordinateDimension();
}

///
///
///
auto
Solid::isEmpty() const -> bool
{
  return exteriorShell().isEmpty();
}

///
///
///
auto
Solid::is3D() const -> bool
{
  return exteriorShell().is3D();
}

///
///
///
auto
Solid::isMeasured() const -> bool
{
  return exteriorShell().isMeasured();
}

///
///
///
void
Solid::accept(GeometryVisitor &visitor)
{
  return visitor.visit(*this);
}

///
///
///
void
Solid::accept(ConstGeometryVisitor &visitor) const
{
  return visitor.visit(*this);
}

} // namespace SFCGAL
