#include "HeightMap.h"


// loads in a 2D black and white image and stores its pixel color values in a 2D vector
// where each row represents the 'x' coordinate, each column the 'z' coordinate 
// and the value stored representing the 'y' coordinate
void loadHeightmap(char *file, std::vector <std::vector <GLfloat>> &Hmap)
{
	SDL_Surface *surface = SDL_LoadBMP(file);
	std::vector <float> temp;
	int bpp = surface->format->BytesPerPixel;
	for (int i = 0; i < surface->h; i++)
	{
		temp.clear();

		for (int j = 0; j < surface->w; j++)
		{
			Uint8 *p = (Uint8 *)surface->pixels + (i*surface->w + j)*bpp;
			temp.push_back(p[2] / 255.0);
		}
		Hmap.push_back(temp);
	}
	SDL_FreeSurface(surface);
}


// genrates Hverts, Hnorms, Hindies, Htexcoords, HvertsCount, HindexCount from the Hmap 
// Hnorms are smooth normals calculated during their generations
void initMesh(std::vector <std::vector <GLfloat>> &Hmap, std::vector <GLfloat> &Hverts,
				std::vector <GLfloat> &Hnorms, std::vector <GLuint> &Hindies,
				std::vector <GLfloat> &Htexcoords, GLuint  &HvertsCount, GLuint  &HindexCount)
{
	//cout << endl << "loop 1" << endl;
	for (int i = 0; i < Hmap.size(); i++)
	{
		for (int j = 0; j < Hmap[0].size(); j++)
		{
			Hverts.push_back(j*0.4);
			Hverts.push_back(Hmap[i][j] * 20);
			Hverts.push_back(i*0.4);
		}
	}
	glm::vec3 Norm;
	glm::vec3 Normloitr;
	for (int i = 0; i<Hmap.size(); i++)
	{
		for (int j = 0; j<Hmap[0].size(); j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					//top left corner
					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A
					glm::vec3 A(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);
					//point B
					glm::vec3 B((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);

					glm::vec3 OA(A - O);
					glm::vec3 OB(B - O);

					Norm = glm::normalize(glm::cross(OA, OB));
				}
				else if (j == Hmap[0].size() - 1)
				{
					//top right corner

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));


					//point A1
					glm::vec3 A2((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);
					//point B1
					glm::vec3 B2(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));


					Norm = glm::normalize(Norm1 + Norm2);
				}
				else
				{
					//top border

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2
					glm::vec3 A2((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);
					//point B2
					glm::vec3 B2(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					//point A3
					glm::vec3 A3(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);
					//point B3
					glm::vec3 B3((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);

					glm::vec3 OA3(A3 - O);
					glm::vec3 OB3(B3 - O);
					glm::vec3 Norm3 = glm::normalize(glm::cross(OA3, OB3));

					Norm = glm::normalize((Norm1 + Norm2 + Norm3));
				}

			}
			else if (i>0 && i<Hmap.size() - 1)
			{
				if (j == 0)
				{
					//left border

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2
					glm::vec3 A2((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);
					//point B2
					glm::vec3 B2((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					//point A3
					glm::vec3 A3(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);
					//point B2
					glm::vec3 B3((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);

					glm::vec3 OA3(A3 - O);
					glm::vec3 OB3(B3 - O);
					glm::vec3 Norm3 = glm::normalize(glm::cross(OA3, OB3));

					Norm = glm::normalize((Norm1 + Norm2 + Norm3));

				}
				else if (j == Hmap[0].size() - 1)
				{
					//right border

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2
					glm::vec3 A2((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);
					//point B2
					glm::vec3 B2((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					//point A3
					glm::vec3 A3((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);
					//point B3
					glm::vec3 B3(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);

					glm::vec3 OA3(A3 - O);
					glm::vec3 OB3(B3 - O);
					glm::vec3 Norm3 = glm::normalize(glm::cross(OA3, OB3));

					Norm = glm::normalize((Norm1 + Norm2 + Norm3));
				}
				else
				{
					//MIDDLE

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2
					glm::vec3 A2((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);
					//point B2
					glm::vec3 B2((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					//point A3
					glm::vec3 A3((j - 1)*0.4, Hmap[i + 1][j - 1] * 20, (i + 1)*0.4);
					//point B3
					glm::vec3 B3(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);

					glm::vec3 OA3(A3 - O);
					glm::vec3 OB3(B3 - O);
					glm::vec3 Norm3 = glm::normalize(glm::cross(OA3, OB3));

					//point A4
					glm::vec3 A4(j*0.4, Hmap[i + 1][j] * 20, (i + 1)*0.4);
					//point B4
					glm::vec3 B4((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);

					glm::vec3 OA4(A4 - O);
					glm::vec3 OB4(B4 - O);
					glm::vec3 Norm4 = glm::normalize(glm::cross(OA4, OB4));

					//point A5
					glm::vec3 A5((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);
					//point B5
					glm::vec3 B5((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);

					glm::vec3 OA5(A5 - O);
					glm::vec3 OB5(B5 - O);
					glm::vec3 Norm5 = glm::normalize(glm::cross(OA5, OB5));

					//point A6
					glm::vec3 A6((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);
					//point B6
					glm::vec3 B6(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);

					glm::vec3 OA6(A6 - O);
					glm::vec3 OB6(B6 - O);
					glm::vec3 Norm6 = glm::normalize(glm::cross(OA6, OB6));

					Norm = glm::normalize((Norm1 + Norm2 + Norm3 + Norm4 + Norm5 + Norm6));
				}
			}
			else
			{
				if (j == 0)
				{
					//bottom left corner

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2	
					glm::vec3 A2((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);
					//point B2
					glm::vec3 B2((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					Norm = glm::normalize((Norm1 + Norm2));
				}
				else if (j == Hmap[0].size() - 1)
				{
					//bottom right corner

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);

					Norm = glm::normalize(glm::cross(OA1, OB1));
				}
				else
				{
					//bottom border

					//point Origin
					glm::vec3 O(j*0.4, Hmap[i][j] * 20, i*0.4);
					//point A1
					glm::vec3 A1(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);
					//point B1
					glm::vec3 B1((j - 1)*0.4, Hmap[i][j - 1] * 20, i*0.4);

					glm::vec3 OA1(A1 - O);
					glm::vec3 OB1(B1 - O);
					glm::vec3 Norm1 = glm::normalize(glm::cross(OA1, OB1));

					//point A2
					glm::vec3 A2((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);
					//point B2
					glm::vec3 B2(j*0.4, Hmap[i - 1][j] * 20, (i - 1)*0.4);

					glm::vec3 OA2(A2 - O);
					glm::vec3 OB2(B2 - O);
					glm::vec3 Norm2 = glm::normalize(glm::cross(OA2, OB2));

					//point A3
					glm::vec3 A3((j + 1)*0.4, Hmap[i][j + 1] * 20, i*0.4);
					//point B3
					glm::vec3 B3((j + 1)*0.4, Hmap[i - 1][j + 1] * 20, (i - 1)*0.4);

					glm::vec3 OA3(A3 - O);
					glm::vec3 OB3(B3 - O);
					glm::vec3 Norm3 = glm::normalize(glm::cross(OA3, OB3));

					Norm = glm::normalize((Norm1 + Norm2 + Norm3));
				}
			}
			Hnorms.push_back(Norm.x);
			Hnorms.push_back(Norm.y);
			Hnorms.push_back(Norm.z);
		}
	}
	for (int i = 0; i < Hmap.size() - 1; i++)
	{
		for (int j = 0; j < Hmap[0].size() - 1; j++)
		{
			Hindies.push_back(i*Hmap[0].size() + j);
			Hindies.push_back((i + 1)*Hmap[0].size() + j);
			Hindies.push_back(i*Hmap[0].size() + j + 1);
			Hindies.push_back((i + 1)*Hmap[0].size() + j + 1);
			Hindies.push_back(i*Hmap[0].size() + j + 1);
			Hindies.push_back((i + 1)*Hmap[0].size() + j);
		}
	}

	for (float i = 0; i<Hmap.size(); i++)
	{
		for (float j = 0; j<Hmap[0].size(); j++)
		{
			Htexcoords.push_back((float)j / (float)(Hmap.size() - 1));
			Htexcoords.push_back((float)i / (float)(Hmap[0].size() - 1));
		}
	}

	HvertsCount = Hverts.size() / 3;
	HindexCount = Hindies.size();

}
