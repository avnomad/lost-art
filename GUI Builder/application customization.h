// NO INCLUDE GUARD!!!

std::vector<geometry::Rectangle<float>> controls(1);

void updateCoordinates(float screenWidth, float screenHeight, float pixelWidth, float pixelHeight)
{
	controls[0].left() = screenWidth/4;
	controls[0].bottom() = screenHeight/4;
	controls[0].right() = 3*screenWidth/4;
	controls[0].top() = 3*screenHeight/4;
} // end function updateCoordinates
