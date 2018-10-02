# PDF_DeepZoom
Welcome to **PDF_DeepZoom** utility.
This tool, writtten in **C**, allows you to create a set of tiles at various resolutions from a **PDF** file using the [C api from the libvips project](http://libvips.github.io/libvips/API/current/using-from-c.html).
This code is based on libvips v8.6.3. 
The version v8.7 is now using the PDF loader based on Google’s PDFium project which is not implemented in this code.

# Build
In order to compile and execute this tool you need to install libvips with PDF support. You'll find the steps at the [project install webpage](http://libvips.github.io/libvips/install.html).
Make sure to have this lines during the configure step:

    PDF import with poppler - glib : yes
    image pyramid export : yes



# Usage
To run this code you must have this folder **/public/images/dzi/** at the root of your working folder. Feel free to adapt the code to fit your needs!
You can compile the code with this line:
>$ gcc -g -Wall -o PDF_DeepZoom PDF_DeepZoom.c \`pkg-config vips --cflags --libs\`

And run it with:
>$ ./PDF_DeepZoom pdfFile outputFolder

 Where pdfFile is the pdf you want to process and outputFolder the folder name where to store the result.
 
 ## Details
 First the pdf file is loaded with  the **vips_pdfload()** function. The resolution is set to 300 dpi because i need a high level of zoom but if you need lower resolution you can set the    **gdouble dpi**  variable at the top of the source code, by default vips_pdfload() use a value of 72 dpi. 
 Be aware that this process is time consuming.

Then the folder to store the result is created  in **/public/images/dzi/** with the name you passed as the second argument.

Next the tiles are generated with the **vips_dzsave()** function. If this process is successful a thumbnail will be created as well.

## Finally
Now you can use this tiles to view your PDF with [leaflet](https://leafletjs.com/) for example or any tools using Deep Zoom concept.
