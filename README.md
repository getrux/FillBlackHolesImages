# FillBlackHolesImages

This is an algorithm to delete objects in a scene and fill that hole with similar colored pixels

This is the original image and we want to delete the cube located in the center of the image

![alt tag](http://i.imgur.com/6VI98Bw.png)

.

We can create the mask image to indicate what part we want to delete

![alt tag](http://i.imgur.com/agVO3cW.png)

Finally, the algorithm to delete the object and fill the hole with new pixels, analyze the contour to try to choose the best combination

![alt tag](http://i.imgur.com/zY3WcxM.png)
