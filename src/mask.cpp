#include "raylib.h"
#include <stdlib.h>

void BuildCollisionMap(Image mask, int alphaThreshold) {
    int w = mask.width;
    int h = mask.height;
    Color* px = LoadImageColors(mask);
    if (!px) { return; }

    unsigned char* solid = (unsigned char*)malloc(w * h);
    for (int y = 0; y < h; y++) 
        for (int x = 0; x < w; x++) {
        Color c = px[y * w + x];
        solid[y * w + x] = (c.a > alphaThreshold) ? 1 : 0;
    }
    UnloadImageColors(px);    

    for (int i = 0; i < w - 1; i++) {
        for (int j = 0; j < h - 1; j++) {
            let x = i * rez;
            let y = j * rez;
            let a = createVector(x + rez * 0.5, y            );
            let b = createVector(x + rez, y + rez * 0.5);
            let c = createVector(x + rez * 0.5, y + rez      );
            let d = createVector(x, y + rez * 0.5);

            let threshold = 90; // map(mouseX, 0, width, 0, 255);
            let c1 = field[i][j] < threshold ? 0 : 1;
            let c2 = field[i+1][j] < threshold ? 0 : 1;
            let c3 = field[i+1][j+1]  < threshold ? 0 : 1;
            let c4 = field[i][j+1] < threshold ? 0 : 1;


            let state = getState(c1, c2, c3, c4);
            stroke(255);
            strokeWeight(4);
            switch (state) {
            case 1: drawLine(c, d); break;
            case 2: drawLine(b, c); break;
            case 3: drawLine(b, d); break;
            case 4: drawLine(a, b); break;
            case 5: drawLine(a, d); drawLine(b, c); break;
            case 6: drawLine(a, c); break;
            case 7: drawLine(a, d); break;
            case 8: drawLine(a, d); break;
            case 9: drawLine(a, c); break;
            case 10: drawLine(a, b); drawLine(c, d); break;
            case 11: drawLine(a, b); break;
            case 12: drawLine(b, d); break;
            case 13: drawLine(b, c); break;
            case 14: drawLine(c, d); break;
            }
    }
  }
      
}