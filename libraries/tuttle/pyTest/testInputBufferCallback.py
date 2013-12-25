# scons: MemoryBuffer Png

from pyTuttle import tuttle
import tempfile

import numpy
from PIL import Image

from nose.tools import *


def setUp():
	tuttle.core().preload(False)


# This is called by Tuttle as an input of the graph
def getImage(time):
	img = numpy.asarray( Image.open("TuttleOFX-data/image/jpeg/MatrixLarge.jpg") )
	return (img.tostring(), img.shape[1], img.shape[0], img.strides[0])


def testInputBufferCallback():

	g = tuttle.Graph()

	ib = g.createInputBuffer()
	ib.setComponents( InputBufferWrapper.ePixelComponentRGB )
	ib.setBitDepth( InputBufferWrapper.eBitDepthUByte )
	ib.setOrientation( InputBufferWrapper.eImageOrientationFromTopToBottom )
	ib.setPyCallback( getImage )

	filepath = tempfile.NamedTemporaryFile( prefix="inputBufferCallback-", suffix=".png" )
	w = g.createNode("tuttle.pngwriter", filename = filepath.name )

	g.connect( ib.getNode(), w )
	g.compute( w )

