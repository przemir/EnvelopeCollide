
const int animation_frame_start[] = {1, 4, 14, 25, 29, 41};

void RenderModel(int frame, float percent, int bron, PKT pos, float kat)
{
	float model_v[MAX_VERTEX*3];
	float model_triv[MAX_TRIANGLES*9];
	float model_v_tex[MAX_TRIANGLES*6];
	int vertex_num = 1144;
	int n = 5898;


	int nframe = next_frame(frame);
	for(int i = 0; i < vertex_num*3; i++)
	{
		model_v[i] = model_vertex[i + frame*vertex_num*3] + (model_vertex[i + nframe*vertex_num*3] - model_vertex[i + frame*vertex_num*3]) * percent;
	}

	n = 404*3;

	for(int i = 0; i < n; i++)
	{
		model_triv[i*9+0] = model_v[model_tri[i*3+0]*3+0];
		model_triv[i*9+1] = model_v[model_tri[i*3+0]*3+1];
		model_triv[i*9+2] = model_v[model_tri[i*3+0]*3+2];
		model_triv[i*9+3] = model_v[model_tri[i*3+1]*3+0];
		model_triv[i*9+4] = model_v[model_tri[i*3+1]*3+1];
		model_triv[i*9+5] = model_v[model_tri[i*3+1]*3+2];
		model_triv[i*9+6] = model_v[model_tri[i*3+2]*3+0];
		model_triv[i*9+7] = model_v[model_tri[i*3+2]*3+1];
		model_triv[i*9+8] = model_v[model_tri[i*3+2]*3+2];
	}



	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z+0.3);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(kat, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);



	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_TFIXED, 0, model_triv);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_TFIXED, 0, model_texCoord);

	glDrawArrays(GL_TRIANGLES, 0, n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
/*
//	glDrawElements(GL_TRIANGLES, (n), GL_UNSIGNED_SHORT, &model_tri[0]);
	if(bron != 0)
	{
	n = (weapon_tri_start[bron+1] - weapon_tri_start[bron])/3;
	for(int i = 0; i < n; i++)
	{
		model_triv[i*9+0] = model_v[model_tri[weapon_tri_start[bron]+i*3+0]*3+0];
		model_triv[i*9+1] = model_v[model_tri[weapon_tri_start[bron]+i*3+0]*3+1];
		model_triv[i*9+2] = model_v[model_tri[weapon_tri_start[bron]+i*3+0]*3+2];
		model_triv[i*9+3] = model_v[model_tri[weapon_tri_start[bron]+i*3+1]*3+0];
		model_triv[i*9+4] = model_v[model_tri[weapon_tri_start[bron]+i*3+1]*3+1];
		model_triv[i*9+5] = model_v[model_tri[weapon_tri_start[bron]+i*3+1]*3+2];
		model_triv[i*9+6] = model_v[model_tri[weapon_tri_start[bron]+i*3+2]*3+0];
		model_triv[i*9+7] = model_v[model_tri[weapon_tri_start[bron]+i*3+2]*3+1];
		model_triv[i*9+8] = model_v[model_tri[weapon_tri_start[bron]+i*3+2]*3+2];
	}
	for(int i = 0; i < n*6; i++)
	{
		model_v_tex[i] = model_texCoord[(weapon_tri_start[bron])*2+i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_TFIXED, 0, model_triv);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_TFIXED, 0, model_v_tex);
	 glDrawArrays(GL_TRIANGLES, 0, n*3);
	}
//	if(bron != 0) glDrawElements(GL_TRIANGLES, (n), GL_UNSIGNED_SHORT, &model_tri[weapon_tri_start[bron]]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
*/
	glPopMatrix();
}


int next_frame(int frame)
{
	int stadium = 0;
	if(frame >= animation_frame_start[0]) stadium = 0;
	if(frame >= animation_frame_start[1]) stadium = 1;
	if(frame >= animation_frame_start[2]) stadium = 2;
	if(frame >= animation_frame_start[3]) stadium = 3;
	if(frame >= animation_frame_start[4]) stadium = 4;
	if(frame == animation_frame_start[stadium+1]-1) return animation_frame_start[stadium];
	else return frame+1;
}