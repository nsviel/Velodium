#ifndef EDL_H
#define EDL_H
&

// A Screen Shader which implements Eye-Dome-Lighting shading for non realistic rendering of point cloud
class shader_EDL
{
public:
  shader_EDL();
  ~shader_EDL();

  // Setup the textures to draw in
  bool SetUpTextures(){

    GLuint first_pass_frame_buffer_ = -1;
    GLuint second_pass_frame_buffer = -1;
    GLuint gl_color_texture_id_ = -1;
    GLuint gl_depth_texture_id_ = -1;
    GLuint gl_post_process_texture_id_;

    // Initializes the Framebuffer to render the scene in a texture
    glGenFramebuffers(1, &first_pass_frame_buffer_);
    glGenFramebuffers(1, &second_pass_frame_buffer);
    glGenTextures(1, &gl_color_texture_id_);
    glGenTextures(1, &gl_depth_texture_id_);
    glGenTextures(1, &gl_post_process_texture_id_);
    BindTextures();

    //Active and bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gl_color_texture_id_);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, gl_depth_texture_id_);

		return true;
  }

  /*		bool with_edl = true;
      double edl_strength = 100.0;
      double edl_distance = 1.0;
      double z_far = 10000.0;
      double z_near = 0.1;

      //EDL shader / Initialize the texture locations
      auto color_texture_loc = glGetUniformLocation(shader_program_ID, "colorTexture");
      auto depth_texture_loc = glGetUniformLocation(shader_program_ID, "depthTexture");
      glUniform1i(color_texture_loc, 0);
      glUniform1i(depth_texture_loc, 1);

      auto a_loc = glGetUniformLocation(shader_program_ID, "A");
      auto b_loc = glGetUniformLocation(shader_program_ID, "B");
      auto a = -(z_far + z_near) / (z_far - z_near);
      auto b = -2 * (z_far * z_near) / (z_far - z_near);
      glUniform1f(a_loc, (float) a);
      glUniform1f(b_loc, (float) b);

      auto edl_strength_loc = glGetUniformLocation(shader_program_ID, "EDL_STRENGTH");
      auto edl_dist_loc = glGetUniformLocation(shader_program_ID, "EDL_DISTANCE");
      auto with_edl_loc = glGetUniformLocation(shader_program_ID, "WITH_EDL");

      glUniform1f(edl_strength_loc, (float) edl_strength);
      glUniform1f(edl_dist_loc, (float) edl_distance);
      auto with_edli = (int) (with_edl);
      glUniform1i(with_edl_loc, with_edli);*/


private:
    bool textures_set_ = false;
    GLint gl_color_texture_ = -1;
    GLint gl_depth_texture_ = -1;
};

#endif
