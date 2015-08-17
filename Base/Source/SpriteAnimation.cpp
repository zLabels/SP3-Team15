#include "SpriteAnimation.h"

CSpriteAnimation::CSpriteAnimation(const std::string &meshName, int row, int col) : Mesh(meshName)
 , m_row(row)
 , m_col(col)
 , m_currentTime(0)
 , m_currentFrame(0)
 , m_playCount(0)
{
 m_anim = NULL;
} 

CSpriteAnimation::~CSpriteAnimation(void)
{
	if(m_anim)
	{
		delete m_anim;
	}
}

void CSpriteAnimation::Update(double dt)
{
	//for(unsigned i = 0; i < collidesprite.size(); ++i)
	//{
	//	if(collidesprite[i]->animActive == true)
	//	{
	//		collidesprite[i]->animeCurrentTime += static_cast<float>(dt);
	//		int numFrame = Math::Max(1,(collidesprite[i]->endFrame - collidesprite[i]->startFrame + 1));
	//		float frameTime = collidesprite[i]->animTime /static_cast<float>(numFrame);
	//		collidesprite[i]->animCurrentFrame = Math::Min(collidesprite[i]->endFrame,collidesprite[i]->startFrame + static_cast<int>(collidesprite[i]->animeCurrentTime/frameTime));
	//		/*std::cout << collidesprite[i]->animCurrentFrame  << std::endl;*/
	//		if(collidesprite[i]->animeCurrentTime >= collidesprite[i]->animTime)
	//		{
	//			//collidesprite[i]->ended = true;
	//			collidesprite[i]->animActive = false;
	//			collidesprite[i]->animeCurrentTime = 0.f;
	//			collidesprite[i]->animCurrentFrame = collidesprite[i]->startFrame;
	//			/*if(m_playCount + 1 <= m_anim->repeatCount || m_anim->repeatCount == 0)
	//			{
	//				++m_playCount;
	//				m_currentTime = 0.f;
	//				m_currentFrame = m_anim->startFrame;
	//				collidesprite[i]->animActive = false;
	//			}*/
	//		}
	//	}
	//} 
	if(m_anim->animActive == true)
	{
		m_anim->animeCurrentTime += static_cast<float>(dt);
		int numFrame = Math::Max(1,(m_anim->endFrame - m_anim->startFrame + 1));
		float frameTime = m_anim->animTime /static_cast<float>(numFrame);
		m_anim->animCurrentFrame = Math::Min(m_anim->endFrame,m_anim->startFrame + static_cast<int>(m_anim->animeCurrentTime/frameTime));
		/*std::cout << collidesprite[i]->animCurrentFrame  << std::endl;*/
		if(m_anim->animeCurrentTime >= m_anim->animTime)
		{
			m_anim->ended = true;

			if(m_anim->repeatCount == 0)
			{
				m_anim->animActive = false;
				m_anim->animeCurrentTime = 0.f;
				m_anim->animCurrentFrame = m_anim->startFrame;
			}
			if(m_anim->repeatCount == 1)
			{
				m_anim->animeCurrentTime = 0.f;
				m_anim->animCurrentFrame = m_anim->startFrame;
			}
		}
	}
}

void CSpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	//if(textureID > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if(mode == DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 0);
	else if(mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);
	else
	{
		if(m_anim->animActive == true)
		{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_anim->animCurrentFrame * 6 * sizeof(GLuint)));
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	{
		glDisableVertexAttribArray(3);
	}
}