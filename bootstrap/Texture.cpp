#include "gl_core_4_4.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace aie {

Texture::Texture() 
	: m_filename("none"),
	m_width(0),
	m_height(0),
	m_glHandle(0),
	m_format(0),
	m_loadedPixels(nullptr) {
}

Texture::Texture(const char * filename)
	: m_filename("none"),
	m_width(0),
	m_height(0),
	m_glHandle(0),
	m_format(0),
	m_loadedPixels(nullptr) {

	load(filename);
}

Texture::Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels)
	: m_filename("none"),
	m_width(width),
	m_height(height),
	m_format(format),
	m_loadedPixels(nullptr) {

	create(width, height, format, pixels);
}

Texture::~Texture() {
	if (m_glHandle != 0)
		glDeleteTextures(1, &m_glHandle);
	if (m_loadedPixels != nullptr)
		stbi_image_free(m_loadedPixels);
}

bool Texture::load(const char* filename) {

	if (m_glHandle != 0) {
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_width = 0;
		m_height = 0;
		m_filename = "none";
	}

	int x = 0, y = 0, comp = 0;
	m_loadedPixels = stbi_load(filename, &x, &y, &comp, STBI_default);

	if (m_loadedPixels != nullptr) {
		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);
		switch (comp) {
		case STBI_grey:
			m_format = RED;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y,
						 0, GL_RED, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_grey_alpha:
			m_format = RG;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y,
						 0, GL_RG, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_rgb:
			m_format = RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y,
						 0, GL_RGB, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_rgb_alpha:
			m_format = RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y,
						 0, GL_RGBA, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		default:	break;
		};
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_width = (unsigned int)x;
		m_height = (unsigned int)y;
		m_filename = filename;
		return true;
	}
	return false;
}

void Texture::create(unsigned int width, unsigned int height, Format format, unsigned char* pixels) {

	if (m_glHandle != 0) {
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_filename = "none";
	}

	m_width = width;
	m_height = height;
	m_format = format;

	glGenTextures(1, &m_glHandle);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (m_format) {
	case RED:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
		break;
	case RG:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, m_width, m_height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
		break;
	case RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		break;
	case RGBA:
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	};

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);
}

} // namespace aie