#include "Engine.h"

using namespace Engine;

bool load_image(Image& image, std::string location)
{
	return image.load(location);
}

//// static variable definition for Resource class
//Dictionary<string, Image> Resources::image_collection(20); // image buffer (subjected to changed to dynamic buffer)
//
//int Resources::load_image(string location)
//{
//	Image image;
//	image.load(location);
//	Pair<string, Image> pair(location, image);
//	image_collection.add(pair);
//	return image_collection.get_size() - 1;
//}
//
//Image& Resources::get_image(string location)
//{
//	int index = image_collection.index_of(location);
//	if (index != -1)
//		return image_collection[index].value;
//	else
//		return image_collection[load_image(location)].value;
//}
