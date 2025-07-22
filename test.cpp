    def plotAIEP_AI_INFER_RESULT_FIRE_TIME(data : AIEP_AI_INFER_RESULT_FIRE_TIME, entries):
        ep_result = data

        figure, axes = plt.subplots()

        axes.set_aspect('equal')

        pa_dict = {}
        radius_m = None

        for row in entries:
            name = row[0].get().strip()
            value = row[1].get().strip()
            if not name or not value:
                continue

            if name.startswith("PA[") and (".lat" in name or ".lon" in name):
                try:
                    idx = int(name.split("[")[1].split("]")[0])
                    coord_type = "lat" if ".lat" in name else "lon"
                    if idx not in pa_dict:
                        pa_dict[idx] = {}
                    pa_dict[idx][coord_type] = float(value)
                except Exception as e:
                    print(f"[경고] 필드 '{name}' 파싱 오류: {e}")

            elif name == "PA[all].radius":
                try:
                    radius_m = float(value)
                except Exception as e:
                    print(f"[경고] 반지름 파싱 오류: {e}")

 
        ax = plt.gca()
        for idx, coord in pa_dict.items():
            if "lat" in coord and "lon" in coord:
                lat = coord["lat"]
                lon = coord["lon"]

                plt.scatter(lon, lat, color='g', s=25, marker='x')
                plt.text(lon + 0.001, lat + 0.001, f"PA[{idx}]", fontsize=8, color='green')

                if radius_m is not None:
                    meters_per_deg_lat = 111320
                    meters_per_deg_lon = 111320 * math.cos(math.radians(lat))

                    radius_lat_deg = radius_m / meters_per_deg_lat
                    radius_lon_deg = radius_m / meters_per_deg_lon

                    # 타원 (ellipse): (center_x, center_y), width (deg), height (deg)
                    ellipse = Ellipse(
                        (lon, lat),
                        width=2 * radius_lon_deg,
                        height=2 * radius_lat_deg,
                        edgecolor='green',
                        facecolor='none',
                        linestyle='--'
                    )
                    ax.add_patch(ellipse)
            else:
                print(f"[경고] PA[{idx}] 위도 또는 경도 누락")

 
        ax = plt.gca()
        for idx, coord in pa_dict.items():
            if "lat" in coord and "lon" in coord:
                lat = coord["lat"]
                lon = coord["lon"]

                plt.scatter(lon, lat, color='g', s=25, marker='x')
                plt.text(lon + 0.001, lat + 0.001, f"PA[{idx}]", fontsize=8, color='green')

                if radius_m is not None:
                    meters_per_deg_lat = 111320
                    meters_per_deg_lon = 111320 * math.cos(math.radians(lat))

                    radius_lat_deg = radius_m / meters_per_deg_lat
                    radius_lon_deg = radius_m / meters_per_deg_lon

                    # 타원 (ellipse): (center_x, center_y), width (deg), height (deg)
                    ellipse = Ellipse(
                        (lon, lat),
                        width=2 * radius_lon_deg,
                        height=2 * radius_lat_deg,
                        edgecolor='green',
                        facecolor='none',
                        linestyle='--'
                    )
                    ax.add_patch(ellipse)
            else:
                print(f"[경고] PA[{idx}] 위도 또는 경도 누락")        

        list_Short_traj_e = [] 
        list_Short_traj_n = []
        list_Early_traj_e = []
        list_Early_traj_n = []
        list_Late_traj_e = []
        list_Late_traj_n = []

        list_target_e = []
        list_target_n = []

        for i in range(128):
            Short_e, Short_n = ep_result.Short_Traj[i].dLongitude, ep_result.Short_Traj[i].dLatitude
            list_Short_traj_e.append(Short_e)
            list_Short_traj_n.append(Short_n)

            Early_e, Early_n = ep_result.Early_Traj[i].dLongitude, ep_result.Early_Traj[i].dLatitude
            list_Early_traj_e.append(Early_e)
            list_Early_traj_n.append(Early_n)

            Late_e, Late_n = ep_result.Late_Traj[i].dLongitude, ep_result.Late_Traj[i].dLatitude
            list_Late_traj_e.append(Late_e)
            list_Late_traj_n.append(Late_n)

            tgt_e, tgt_n = ep_result.Target_Traj[i].dLongitude, ep_result.Target_Traj[i].dLatitude
            list_target_e.append(tgt_e)
            list_target_n.append(tgt_n)

        axes.plot(list_Short_traj_e, list_Short_traj_n, color='red')
        axes.plot(list_Early_traj_e, list_Early_traj_n, color='green')
        axes.plot(list_Late_traj_e, list_Late_traj_n, color='blue')
        axes.plot(list_target_e, list_target_n, color='grey')

        axes.scatter(ep_result.Short_LaunchPoint.dLongitude, ep_result.Short_LaunchPoint.dLatitude, color='red')
        axes.scatter(ep_result.Early_LaunchPoint.dLongitude, ep_result.Early_LaunchPoint.dLatitude, color='green')
        axes.scatter(ep_result.Late_LaunchPoint.dLongitude, ep_result.Late_LaunchPoint.dLatitude, color='blue')

        axes.scatter(ep_result.Short_HitPoint.dLongitude, ep_result.Short_HitPoint.dLatitude, color='red')
        axes.scatter(ep_result.Early_HitPoint.dLongitude, ep_result.Early_HitPoint.dLatitude, color='green')
        axes.scatter(ep_result.Late_HitPoint.dLongitude, ep_result.Late_HitPoint.dLatitude, color='blue')

        plt.show()
