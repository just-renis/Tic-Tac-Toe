#pragma once
#include "Program.h"
#include "P3_Bird.h"

class P3_Pipe
{
public:
	P3_Pipe(const pair<RectangleShape, RectangleShape>& body);
	static void pipeLogic(Clock& pipeGenerationTime, vector<P3_Pipe*>& pipes, vector<P3_Bird*>& birds);
	static void draw(RenderWindow& window, const vector<P3_Pipe*> pipes);
private:
	pair<RectangleShape, RectangleShape> body;
	static P3_Pipe* generatePipe();
	void movement();
	static void collision(vector<P3_Pipe*>& pipes, vector<P3_Bird*>& birds);
};