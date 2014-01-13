all:
	gcc -DPNG_IMAGE_TEST -g -lpng png_image.c -o png_r_p_w
	./png_r_p_w rgb.png rgb_out.png
	./png_r_p_w rgba.png rgba_out.png

clean:
	rm -f png_r_p_w *out.png

	
