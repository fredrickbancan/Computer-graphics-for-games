#pragma once

#include <string>

namespace aie {

// a class for wrapping up an opengl texture image
class Texture {
public:

	enum Format : unsigned int {
		RED	= 1,
		RG,
		RGB,
		RGBA
	};

	Texture();
	Texture(const char* filename);
	Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);
	virtual ~Texture();

	// load a jpg, bmp, png or tga
	bool load(const char* filename);

	// creates a texture that can be filled in with pixels
	void create(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);

	// returns the filename or "none" if not loaded from a file
	const std::string& getFilename() const { return m_filename; }

	// binds the texture to the specified slot
	void bind(unsigned int slot) const;

	// returns the opengl texture handle
	unsigned int getHandle() const { return m_glHandle; }

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	unsigned int getFormat() const { return m_format; }
	const unsigned char* getPixels() const { return m_loadedPixels; }

protected:

	std::string		m_filename;
	unsigned int	m_width;
	unsigned int	m_height;
	unsigned int	m_glHandle;
	unsigned int	m_format;
	unsigned char*	m_loadedPixels;
};

} // namespace aie