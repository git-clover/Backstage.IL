#include <catch2/catch_all.hpp>

#include <cmath>
#include <iostream>
#include <boost/filesystem.hpp>
#include "libslic3r/libslic3r.h"
#include <vector>
#include "libslic3r/Polygon.hpp"
#include <math.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "libslic3r/ClipperUtils.hpp"
#include "libslic3r/ExPolygon.hpp"
#include "libslic3r/SVG.hpp"

using namespace Slic3r;

// #define TESTS_EXPORT_SVGS

SCENARIO("Constant offset", "[ClipperUtils]") {
	coord_t s = 1000000;
	GIVEN("20 mm box") {
		ExPolygon box_20_mm;
		box_20_mm.contour.points = { Vec2crd{ 0, 0 }, Vec2crd{ 20 * s, 0 }, Vec2crd{ 20 * s, 20 * s}, Vec2crd{ 0, 20 * s} };
		std::vector<float> deltas_plus(box_20_mm.contour.points.size(), 1. * s);
		std::vector<float> deltas_minus(box_20_mm.contour.points.size(), - 1. * s);
		Polygons output;
		WHEN("Slic3r::offset()") {
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("plus 1 mm, miter " << miter << "x") {
					output = Slic3r::offset(box_20_mm, 1. * s, jtMiter, miter);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("constant_offset_box_20_mm_plus_1_mm_miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(box_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area is 22.2 mm2") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(22. * 22. * s * s));
					}
				}
				DYNAMIC_SECTION("minus 1 mm, miter " << miter << "x") {
					output = Slic3r::offset(box_20_mm, - 1. * s, jtMiter, miter);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("constant_offset_box_20_mm_minus_1_mm_miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(box_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area is 18.2 mm2") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(18. * 18. * s * s));
					}
				}
			}
		}
		WHEN("Slic3r::variable_offset_outer/inner") {
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("plus 1 mm, miter " << miter << "x") {
					output = Slic3r::variable_offset_outer(box_20_mm, { deltas_plus }, miter);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("variable_offset-box_20_mm-plus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(box_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area is 22.2 mm2") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(22. * 22. * s * s));
					}
				}
				DYNAMIC_SECTION("minus 1 mm, miter " << miter << "x") {
					output = Slic3r::variable_offset_inner(box_20_mm, { deltas_minus }, miter);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("variable_offset-box_20_mm-minus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(box_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area is 18.2 mm2") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(18. * 18. * s * s));
					}
				}
			}
		}
	}

	GIVEN("20 mm box with 10 mm hole") {
		ExPolygon box_20_mm;
		box_20_mm.contour.points = { Vec2crd{ 0, 0 }, Vec2crd{ 20 * s, 0 }, Vec2crd{ 20 * s, 20 * s}, Vec2crd{ 0, 20 * s} };
		box_20_mm.holes.emplace_back(Slic3r::Polygon({ Vec2crd{ 5 * s, 5 * s }, Vec2crd{ 5 * s, 15 * s}, Vec2crd{ 15 * s, 15 * s}, Vec2crd{ 15 * s, 5 * s } }));
		std::vector<float> deltas_plus(box_20_mm.contour.points.size(), 1. * s);
		std::vector<float> deltas_minus(box_20_mm.contour.points.size(), -1. * s);
		ExPolygons output;
		SECTION("Slic3r::offset()") {
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("miter " << miter << "x") {
					WHEN("plus 1 mm") {
						output = Slic3r::offset_ex(box_20_mm, 1. * s, jtMiter, miter);
#ifdef TESTS_EXPORT_SVGS
						{
							SVG svg(debug_out_path("constant_offset-box_20_mm-10_mm_hole-plus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
							svg.draw(box_20_mm, "blue");
							svg.draw_outline(to_polygons(output), "black", coord_t(scale_(0.01)));
						}
#endif
						THEN("Area is 22^2-8^2 mm2") {
							REQUIRE(output.size() == 1);
							REQUIRE(output.front().area() == Catch::Approx((22. * 22. - 8. * 8.) * s * s));
						}
					}
					WHEN("minus 1 mm") {
						output = Slic3r::offset_ex(box_20_mm, - 1. * s, jtMiter, miter);
#ifdef TESTS_EXPORT_SVGS
						{
							SVG svg(debug_out_path("constant_offset-box_20_mm-10_mm_hole-minus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
							svg.draw(box_20_mm, "blue");
							svg.draw_outline(to_polygons(output), "black", coord_t(scale_(0.01)));
						}
#endif
						THEN("Area is 18^2-12^2 mm2") {
							REQUIRE(output.size() == 1);
							REQUIRE(output.front().area() == Catch::Approx((18. * 18. - 12. * 12.) * s * s));
						}
					}
				}
			}
		}
		SECTION("Slic3r::variable_offset_outer()") {
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("miter " << miter << "x") {
					WHEN("plus 1 mm") {
						output = Slic3r::variable_offset_outer_ex(box_20_mm, { deltas_plus, deltas_plus }, miter);
#ifdef TESTS_EXPORT_SVGS
						{
							SVG svg(debug_out_path("variable_offset-box_20_mm-10_mm_hole-plus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
							svg.draw(box_20_mm, "blue");
							svg.draw_outline(to_polygons(output), "black", coord_t(scale_(0.01)));
						}
#endif
						THEN("Area is 22^2-8^2 mm2") {
							REQUIRE(output.size() == 1);
							REQUIRE(output.front().area() == Catch::Approx((22. * 22. - 8. * 8.) * s * s));
						}
					}
					WHEN("minus 1 mm") {
						output = Slic3r::variable_offset_inner_ex(box_20_mm, { deltas_minus, deltas_minus }, miter);
#ifdef TESTS_EXPORT_SVGS
						{
							SVG svg(debug_out_path("variable_offset-box_20_mm-10_mm_hole-minus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
							svg.draw(box_20_mm, "blue");
							svg.draw_outline(to_polygons(output), "black", coord_t(scale_(0.01)));
						}
#endif
						THEN("Area is 18^2-12^2 mm2") {
							REQUIRE(output.size() == 1);
							REQUIRE(output.front().area() == Catch::Approx((18. * 18. - 12. * 12.) * s * s));
						}
					}
				}
			}
		}
	}

	GIVEN("20 mm right angle triangle") {
		ExPolygon triangle_20_mm;
		triangle_20_mm.contour.points = { Vec2crd{ 0, 0 }, Vec2crd{ 20 * s, 0 }, Vec2crd{ 0, 20 * s } };
		Polygons output;
		double offset = 1.;
		// Angle of the sharp corner bisector.
		double angle_bisector = M_PI / 8.;
		// Area tapered by mitering one sharp corner.
		double area_tapered = pow(offset * (1. / sin(angle_bisector) - 1.), 2.) * tan(angle_bisector);
		double l_triangle_side_offsetted = 20. + offset * (1. + 1. / tan(angle_bisector));
		double area_offsetted = (0.5 * l_triangle_side_offsetted * l_triangle_side_offsetted - 2. * area_tapered) * s * s;
		SECTION("Slic3r::offset()") {
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("Outer offset 1 mm, miter " << miter << "x") {
					output = Slic3r::offset(triangle_20_mm, offset * s, jtMiter, 2.0);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("constant_offset_triangle_20_mm_plus_1_mm_miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(triangle_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area matches") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(area_offsetted));
					}
				}
			}
		}
		SECTION("Slic3r::variable_offset_outer()") {
			std::vector<float> deltas(triangle_20_mm.contour.points.size(), 1. * s);
			for (double miter : { 2.0, 1.5, 1.2 }) {
				DYNAMIC_SECTION("Outer offset 1 mm, miter " << miter << "x") {
					output = Slic3r::variable_offset_outer(triangle_20_mm, { deltas }, 2.0);
#ifdef TESTS_EXPORT_SVGS
					{
						SVG svg(debug_out_path("variable_offset-triangle_20_mm-plus_1_mm-miter%lf.svg", miter).c_str(), get_extents(output));
						svg.draw(triangle_20_mm, "blue");
						svg.draw_outline(output, "black", coord_t(scale_(0.01)));
					}
#endif
					THEN("Area matches") {
						REQUIRE(output.size() == 1);
						REQUIRE(output.front().area() == Catch::Approx(area_offsetted));
					}
				}
			}
		}
	}
}
