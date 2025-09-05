def plotSituation(self, textOut):
        
        tor_launch_angle, t, HIT_E, HIT_N = calculate_launch_angle(
            self.env.PosList_tgtE[0], self.env.PosList_tgtN[0],
            self.env.Target.CurCourse, self.env.Target.CurSpeed, self.env.Torpedo.CurSpeed
        )
        
        t_values = np.linspace(0, t, 10)
        ccm_tor_list_e = float(self.env.Torpedo.CurSpeed * np.sin(tor_launch_angle * DEG2RAD)) * t_values
        ccm_tor_list_n = float(self.env.Torpedo.CurSpeed * np.cos(tor_launch_angle * DEG2RAD)) * t_values
        
        
        fig, ax0 = plt.subplots(1, 1, figsize=(6, 6), 
                               facecolor=self.color_config.get_color('background'))
        ax0.set_facecolor(self.color_config.get_color('background'))
        
        
        ax0.grid(color=self.color_config.get_color('grid'), alpha=0.7)
        ax0.set_xlim([-1.0, 2.5])
        ax0.set_ylim([-1.0, 2.5])
        ax0.set_aspect('equal')
        
        
        
        
        
        ax0.tick_params(colors=self.color_config.get_color('axis_label'))
        for spine in ax0.spines.values():
            spine.set_color(self.color_config.get_color('axis_label'))
        
        
        ax0.plot(ccm_tor_list_e, ccm_tor_list_n, 
                label='CCM', 
                color=self.color_config.get_color('torpedo_ccm_path'))
        
        
        cmap = self.color_config.create_pa_colormap()
        
        circles = []
        for pa in self.env.m_pa:
            circle = plt.Circle((pa.E, pa.N), pa.Radius,
                                facecolor=(1, 1, 1, 0),  
                                edgecolor=self.color_config.get_color('pa_circle_edge'), 
                                alpha=0.6)
            ax0.add_artist(circle)
            circles.append(circle)
        
        ax0.plot(self.env.PosList_tgtE[0], self.env.PosList_tgtN[0], 
                'o', 
                color=self.color_config.get_color('target_start'))
        ax0.plot(self.env.PosList_torE[0], self.env.PosList_torN[0], 
                'o', 
                color=self.color_config.get_color('torpedo_start'))
        
        
        target_line, = ax0.plot([], [], label='Target', 
                               color=self.color_config.get_color('target_trajectory'))
        torpedo_line, = ax0.plot([], [], label='Torpedo', 
                                color=self.color_config.get_color('torpedo_trajectory'))
        
        
        legend = ax0.legend()
        legend.get_frame().set_facecolor(self.color_config.get_color('background'))
        for text in legend.get_texts():
            text.set_color(self.color_config.get_color('legend'))



        def update(frame):
            
            if frame < len(self.AttnList_PA):
                weights = np.array(self.AttnList_PA[frame], dtype=np.float32)
            else:
                weights = np.zeros(len(self.env.m_pa), dtype=np.float32)
            
            weights = np.clip(weights, 0.0, 1.0)
            max_w = np.max(weights)
            if max_w > 0:
                weights = weights / (max_w + 1e-8)
            
            
            for idx, circle in enumerate(circles):
                facecolor = cmap(weights[idx])
                circle.set_facecolor(facecolor)
            
            target_line.set_data(self.env.PosList_tgtE[:frame], self.env.PosList_tgtN[:frame])
            torpedo_line.set_data(self.env.PosList_torE[:frame], self.env.PosList_torN[:frame])
            
            return [target_line, torpedo_line] + circles
        
        total_frames = min(len(self.env.PosList_tgtE), len(self.env.PosList_torE), len(self.AttnList_PA))
        ani = animation.FuncAnimation(
            fig,
            update,
            frames=total_frames,
            repeat=False
        )
        
        secSinceAppStart = int(time.time() - g_AppStartTime)
        filename = f'./output_plot_gif_paFix16/plot_{secSinceAppStart}.gif'
        ani.save(filename, writer='imagemagick', fps=10)
        plt.close()
        return filename		
