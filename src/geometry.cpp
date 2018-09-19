#include <setsuna/rtti_prefix.h>
#include <setsuna/geometry.h>

namespace setsuna {

ref<mesh> geometry::sphere(float radius,
                           uint32_t width_segments, uint32_t height_segments,
                           float phi_length, float theta_length) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;

	std::vector<std::vector<uint32_t>> grid;
	std::vector<uint32_t> grid_row(width_segments + 1);
	uint32_t index = 0;

	for (uint32_t iy = 0; iy <= height_segments; iy++) {
		auto v = float(iy) / height_segments;
		grid_row.clear();

		for (uint32_t ix = 0; ix <= width_segments; ix++) {
			auto u = float(ix) / width_segments;

			positions.emplace_back(
			  -radius * glm::cos(u * phi_length) * glm::sin(v * theta_length),
			  radius * glm::cos(v * theta_length),
			  radius * glm::sin(u * phi_length) * glm::sin(v * theta_length));

			normals.emplace_back(glm::normalize(positions.back()));

			texcoords.emplace_back(u, 1 - v);

			grid_row.push_back(index++);
		}
		grid.push_back(std::move(grid_row));
	}

	for (uint32_t iy = 0; iy < height_segments; iy++) {
		for (uint32_t ix = 0; ix < width_segments; ix++) {
			auto a = grid[iy][ix + 1];
			auto b = grid[iy][ix];
			auto c = grid[iy + 1][ix];
			auto d = grid[iy + 1][ix + 1];

			if (iy != 0) {
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);
			}
			if (iy != height_segments - 1 || theta_length < glm::pi<float>()) {
				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}

	attribute a0(3, GL_FLOAT, std::move(positions));
	attribute a1(2, GL_FLOAT, std::move(texcoords));
	attribute a2(3, GL_FLOAT, std::move(normals));
	auto mesh = mesh::create(true, a0, a1, a2);
	mesh->set_indices(indices);
	mesh->calculate_bounding_box(a0.data);
	return mesh;
}

ref<mesh> geometry::plane(float width, float height,
                          uint32_t width_segments, uint32_t height_segments) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;

	auto width_half = width / 2, height_half = height / 2;

	auto segment_width = width / width_segments,
	     segment_height = height / height_segments;

	for (uint32_t iy = 0; iy < height_segments + 1; iy++) {
		auto y = iy * segment_height - height_half;

		for (uint32_t ix = 0; ix < width_segments + 1; ix++) {
			auto x = ix * segment_width - width_half;

			positions.emplace_back(x, y, 0);

			normals.emplace_back(0, 0, 1);

			texcoords.emplace_back(float(ix) / width_segments,
			                       1 - (float(iy) / height_segments));
		}
	}

	for (uint32_t iy = 0; iy < height_segments; iy++) {
		for (uint32_t ix = 0; ix < width_segments; ix++) {
			auto a = ix + (width_segments + 1) * iy;
			auto b = ix + (width_segments + 1) * (iy + 1);
			auto c = (ix + 1) + (width_segments + 1) * (iy + 1);
			auto d = (ix + 1) + (width_segments + 1) * iy;

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);
			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	attribute a0(3, GL_FLOAT, std::move(positions));
	attribute a1(2, GL_FLOAT, std::move(texcoords));
	attribute a2(3, GL_FLOAT, std::move(normals));
	auto mesh = mesh::create(true, a0, a1, a2);
	mesh->set_indices(indices);
	mesh->calculate_bounding_box(a0.data);
	return mesh;
}

}  // namespace setsuna
