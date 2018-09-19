#include <setsuna/rtti_prefix.h>
#include <setsuna/logger.h>

namespace setsuna {

resource::resource() :
    m_ref_count{0} {}

resource::~resource() {
	if (m_ref_count > 0) {
		LOG_DEBUG("Destructing a resource with reference count %d", m_ref_count)
	}
}

void resource::ref() const noexcept {
	++m_ref_count;
}

void resource::unref() const noexcept {
	--m_ref_count;
	if (m_ref_count == 0) {
		delete this;
	}
}

}  // namespace setsuna
