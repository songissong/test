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
