demo: WarIFFChunk.o GameWindow.o dVertex.o dScreen.o dPolygon.o dPart.o dPart3DSceneTwist.o dPart3DSceneSpin2.o dPart3DSceneDots.o dPart3DSceneDist2.o dPart3DScene.o dPart3DScene2.o dPalette.o dObject.o dMaterial.o dLine.o dFace.o dDot.o dContourObject.o dClut2.o dClut1.o dBitmap.o dApp.o SawTeeth/lib/source/wave.o SawTeeth/lib/source/txt.o SawTeeth/lib/source/song.o SawTeeth/lib/source/player.o SawTeeth/lib/source/insply.o stsp.o
	g++ -o demo -lbe -lgame -ltranslation WarIFFChunk.o GameWindow.o dVertex.o dScreen.o dPolygon.o dPart.o dPart3DSceneTwist.o dPart3DSceneSpin2.o dPart3DSceneDots.o dPart3DSceneDist2.o dPart3DScene.o dPart3DScene2.o dPalette.o dObject.o dMaterial.o dLine.o dFace.o dDot.o dContourObject.o dClut2.o dClut1.o dBitmap.o dApp.o SawTeeth/lib/source/wave.o SawTeeth/lib/source/txt.o SawTeeth/lib/source/song.o SawTeeth/lib/source/player.o SawTeeth/lib/source/insply.o stsp.o -lmedia

%.o : sources/%.cpp
	g++ -g -c $< -o $@ -Iheaders -Ist

Sawteeth/lib/source/%.o: Sawteeth/lib/source/%.cpp
	g++ -c $< -o $@
