#include "map.h"
#include <iostream>
#include <queue>

map::map(float tilesS, sf::Vector2f pos) : tilesSizes(tilesS), position(pos)
{
	vis = NULL;
	targetTex.loadFromFile("../img/target.png");
	lineTex[0].loadFromFile("../img/line.png");
	lineTex[1].loadFromFile("../img/curve.png");
	rangeTex[0].loadFromFile("../img/edge.png");
	rangeTex[1].loadFromFile("../img/edge_curved.png");
	inRangeTex.loadFromFile("../img/in_range.png");
	tileData data;
	data.texture.loadFromFile("../img/wall.png");
	data.color = sf::Color(127, 127, 127, 255);
	data.type = '1';
	tilesData.push_back(data);
	data.texture.loadFromFile("../img/wall2.png");
	data.color = sf::Color(195, 195, 195, 255);
	data.type = '1';
	tilesData.push_back(data);
	data.texture.loadFromFile("../img/grass.png");
	data.color = sf::Color(0, 255, 0, 255);
	data.type = '0';
	tilesData.push_back(data);
	data.texture.loadFromFile("../img/grass2.png");
	data.color = sf::Color(0, 128, 0, 255);
	data.type = '0';
	tilesData.push_back(data);
	data.texture.loadFromFile("../img/floor.png");
	data.color = sf::Color(0, 0, 0, 255);
	data.type = '0';
	tilesData.push_back(data);
	data.texture.loadFromFile("../img/water.png");
	data.color = sf::Color(0, 0, 255, 255);
	data.type = '1';
	tilesData.push_back(data);
}

void map::print()
{
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			std::cout << tiles[i][j];
		}
		std::cout << std::endl;
	}
}

void map::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			tilesSprites[i][j].setScale(tilesSizes / 30.0f, tilesSizes / 30.0f);
			tilesSprites[i][j].setPosition(sf::Vector2f(position.x + j * tilesSizes, position.y + i * tilesSizes));
			window.draw(tilesSprites[i][j]);
		}
	}
}

void map::scroll(int sizeDiff, sf::Vector2f posDiff)
{
	if (sizeDiff != 0)
	{
		float scale = tilesSizes;
		tilesSizes = (tilesSizes + sizeDiff) > 10 && (tilesSizes + sizeDiff) < 65 ? tilesSizes + sizeDiff : tilesSizes;
		scale = tilesSizes / scale - 1;
		position += sf::Vector2f(posDiff.x*scale, posDiff.y*scale);
	}
	else
	{
		position -= sf::Vector2f(posDiff.x, posDiff.y);
	}
}

std::vector<sf::Vector2i> map::drawPath(sf::Vector2i start, sf::Vector2i dest, sf::RenderWindow& window)
{
	static sf::Vector2i lastStartPos;
	if (!canPass(dest.y, dest.x) || start == dest)
	{
		return std::vector<sf::Vector2i>(1, sf::Vector2i(0, 0));
	}
	int dx[] = { -1,0,0,1 };
	int dy[] = { 0,-1,1,0 };
	if (lastStartPos != start)
	{
		std::queue<sf::Vector2i> q;
		q.push(sf::Vector2i(start.y, start.x));
		vis = new int*[size.x];
		for (int i = 0; i < size.y; i++)
		{
			vis[i] = new int[size.x];
			for (int j = 0; j < size.x; j++)
				vis[i][j] = -1;
		}
		vis[start.y][start.x] = 0;
		while (!q.empty())
		{
			sf::Vector2i n = q.front();
			q.pop();
			for (int i = 0; i < 4; i++)
			{
				//if (n.x + dx[i] < 0 || n.x + dx[i] >= size.x || n.y + dy[i] < 0 || n.y + dy[i] >= size.y || vis[n.x+dx[i]][n.y+dy[i]] != -1 || tiles[n.x + dx[i]][n.y + dy[i]] != 48)
				if (n.x + dx[i] < 0 || n.x + dx[i] >= size.y || n.y + dy[i] < 0 || n.y + dy[i] >= size.x || vis[n.x + dx[i]][n.y + dy[i]] != -1 || tiles[n.x + dx[i]][n.y + dy[i]] != 48)
					continue;
				q.push(sf::Vector2i(n.x + dx[i], n.y + dy[i]));
				vis[n.x + dx[i]][n.y + dy[i]] = vis[n.x][n.y] + 1;
			}
		}
		lastStartPos = start;
	}
	/*
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			std::cout << vis[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	*/
	if (vis[dest.y][dest.x] == -1)
	{
		std::cout << "unreachable" << std::endl;
		return std::vector<sf::Vector2i>(1, sf::Vector2i(0, 0));
	}
	sf::Vector2i n = sf::Vector2i(dest.y, dest.x);
	int last = -1;
	sf::Sprite line;
	line.setTexture(lineTex[0]);
	line.setScale(tilesSizes / 10, tilesSizes / 10);
	sf::Sprite curve;
	curve.setTexture(lineTex[1]);
	curve.setScale(tilesSizes / 10, tilesSizes / 10);
	std::vector<sf::Vector2i> next;
	while (sf::Vector2i(n.y, n.x) != start)
	{
		for (int i = 0; i < 4; i++)
		{
			//if (n.x + dx[i] < 0 || n.x + dx[i] >= size.x || n.y + dy[i] < 0 || n.y + dy[i] >= size.y || vis[n.x + dx[i]][n.y + dy[i]]==-1 || vis[n.x + dx[i]][n.y + dy[i]] != vis[n.x][n.y]-1)
			if (n.x + dx[i] < 0 || n.x + dx[i] >= size.y || n.y + dy[i] < 0 || n.y + dy[i] >= size.x || vis[n.x + dx[i]][n.y + dy[i]] == -1 || vis[n.x + dx[i]][n.y + dy[i]] != vis[n.x][n.y] - 1)
				continue;
			n = sf::Vector2i(n.x + dx[i], n.y + dy[i]);
			next.push_back(sf::Vector2i(dy[i], dx[i]));
			break;
		}
	}
	n = dest + next[0];
	for (int i = 0; i < next.size() - 1; i++)
	{
		if ((next[i].x == -1 && next[i + 1].x == -1) || (next[i].x == 1 && next[i + 1].x == 1))
		{
			line.setOrigin(4.5, 4.5);
			line.setRotation(0);
			line.setOrigin(0, 0);
			line.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
			window.draw(line);
		}
		else if ((next[i].y == -1 && next[i + 1].y == -1) || (next[i].y == 1 && next[i + 1].y == 1))
		{
			line.setOrigin(4.5, 4.5);
			line.setRotation(90);
			line.setOrigin(0, 10);
			line.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
			window.draw(line);
		}
		else if (next[i].y == -1)
		{
			if (next[i + 1].x == 1)
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(-90);
				curve.setOrigin(10, 0);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
			else	//next[i + 1].x == -1
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(0);
				curve.setOrigin(0, 0);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
		}
		else if (next[i].y == 1)
		{
			if (next[i + 1].x == 1)
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(180);
				curve.setOrigin(10, 10);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
			else	//next[i + 1].x == -1
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(90);
				curve.setOrigin(0, 10);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
		}
		else if (next[i].x == -1)
		{
			if (next[i + 1].y == 1)
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(-90);
				curve.setOrigin(10, 0);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
			else
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(180);
				curve.setOrigin(10, 10);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
		}
		else if (next[i].x == 1)
		{
			if (next[i + 1].y == 1)
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(0);
				curve.setOrigin(0, 0);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
			else
			{
				curve.setOrigin(4.5, 4.5);
				curve.setRotation(90);
				curve.setOrigin(0, 10);
				curve.setPosition(sf::Vector2f(position.x + n.x*tilesSizes, position.y + n.y*tilesSizes));
				window.draw(curve);
			}
		}
		n += next[i + 1];
	}
	sf::Sprite target;
	target.setTexture(targetTex);
	target.setPosition(sf::Vector2f(position.x + dest.x*tilesSizes, position.y + dest.y*tilesSizes));
	target.setScale(tilesSizes / 10, tilesSizes / 10);
	window.draw(target);
	return next;
}

bool map::canPass(int x, int y)
{
	if (tiles[x][y] == '0')
		return true;
	return false;
}

int map::loadFromBitmap(sf::Image image)
{
	size = image.getSize();
	tiles = new int*[size.y];
	tilesSprites = new sf::Sprite*[size.y];
	for (int i = 0; i < size.y; i++)
	{
		tiles[i] = new int[size.x];
		tilesSprites[i] = new sf::Sprite[size.x];
	}
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			auto it = tilesData.begin();
			while (it->color != image.getPixel(j, i))
			{
				if (++it == tilesData.end())
				{
					it--;
					//std::cout << (int)image.getPixel(j, i).r << ' ' << (int)image.getPixel(j, i).g << ' ' << (int)image.getPixel(j, i).b << (int)image.getPixel(j, i).a << std::endl;
					return -1;
				}

			}
			tiles[i][j] = it->type;
			tilesSprites[i][j].setTexture(it->texture);
		}
	}
	return 0;
}

void map::drawRange(int range, sf::Vector2i startPos, sf::RenderWindow& window)
{
	if (vis == NULL)//this should never happen
	{
		std::cout << "drawRange assert failed" << std::endl;
		return;
	}
	sf::Sprite edge;
	int dx[] = { -1,0,0,1 };
	int dy[] = { 0,-1,1,0 };
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			if (vis[i][j] == -1 || vis[i][j] == range + 1)
			{
				int entrances_n = 0;
				for (int k = 0; k < 4; k++)
				{
					sf::Vector2i n = sf::Vector2i(i, j);
					n.x += dx[k];
					n.y += dy[k];
					if (n.x >= 0 && n.x < size.y && n.y >= 0 && n.y < size.x && vis[n.x][n.y] <= range && vis[n.x][n.y] != -1)
					{
						switch (k)
						{
						case 0:
							edge.setOrigin(4.5, 4.5);
							edge.setRotation(90);
							edge.setOrigin(0, 30);
							break;
						case 1:
							edge.setOrigin(4.5, 4.5);
							edge.setRotation(0);
							edge.setOrigin(0, 0);
							break;
						case 2:
							edge.setOrigin(4.5, 4.5);
							edge.setRotation(180);
							edge.setOrigin(30, 30);
							break;
						case 3:
							edge.setOrigin(4.5, 4.5);
							edge.setRotation(-90);
							edge.setOrigin(30, 0);
							break;
						}
						entrances_n++;
						edge.setTexture(rangeTex[0]);
						edge.setScale(tilesSizes / 30.0f, tilesSizes / 30.0f);
						edge.setPosition(position.x + n.y * tilesSizes, position.y + n.x * tilesSizes);
						window.draw(edge);
					}
				}
			}
			if (vis[i][j] <= range)
			{
				edge.setTexture(inRangeTex);
				edge.setScale(tilesSizes / 30.0f, tilesSizes / 30.0f);
				edge.setPosition(position.x + j * tilesSizes, position.y + i * tilesSizes);
				window.draw(edge);
				int x = 0;
				for (int k = 0; k < 4; k++)
				{
					sf::Vector2i n = sf::Vector2i(i, j);
					n.x += dx[k];
					n.y += dy[k];
					if (n.x >= 0 && n.x < size.y && n.y >= 0 && n.y < size.x && (vis[n.x][n.y] > range || vis[n.x][n.y] == -1))
					{
						x++;
						break;
					}
				}
				if (x == 0)
				{
					int dx2[] = { 1,1,-1,-1 };
					int dy2[] = { 1,-1,1,-1 };
					for (int k = 0; k < 4; k++)
					{
						sf::Vector2i n = sf::Vector2i(i, j);
						n.x += dx2[k];
						n.y += dy2[k];
						if (n.x >= 0 && n.x < size.y && n.y >= 0 && n.y < size.x && (vis[n.x][n.y] > range || vis[n.x][n.y] == -1))
						{
							switch (k)
							{
							case 0:
								edge.setOrigin(4.5, 4.5);
								edge.setRotation(180);
								edge.setOrigin(30, 30);
								break;
							case 1:
								edge.setOrigin(4.5, 4.5);
								edge.setRotation(-90);
								edge.setOrigin(30, 0);
								break;
							case 2:
								edge.setOrigin(4.5, 4.5);
								edge.setRotation(90);
								edge.setOrigin(0, 30);
								break;
							case 3:
								edge.setOrigin(4.5, 4.5);
								edge.setRotation(0);
								edge.setOrigin(0, 0);
								break;
							}
							edge.setTexture(rangeTex[1]);
							edge.setScale(tilesSizes / 30.0f, tilesSizes / 30.0f);
							edge.setPosition(position.x + (n.y - dy2[k]) * tilesSizes, position.y + (n.x - dx2[k]) * tilesSizes);
							window.draw(edge);
						}
					}
				}
			}
		}
	}
}