int CWgtEngageManager::updateAndGetHeadingToNextPoint(
    const SPOINT_ENU& curPos,
    const std::array<SPOINT_ENU, C_MAX_EXT_TRAJ>& i_TrajTor,
    int& io_NextTorTrajectoryPoint,
    double& o_HeadingDeg,
    double reachThreshold /* = 2.0 */)
{
    // 경로 끝에 도달한 경우
    if (io_NextTorTrajectoryPoint >= C_MAX_EXT_TRAJ) {
        // 마지막 두 점으로부터 유지할 침로 계산
        const auto& prev = i_TrajTor[C_MAX_EXT_TRAJ - 2];
        const auto& last = i_TrajTor[C_MAX_EXT_TRAJ - 1];

        double dE = last.E - prev.E;
        double dN = last.N - prev.N;

        double headingRad = std::atan2(dE, dN);
        double headingDeg = headingRad * 180.0 / M_PI;
        if (headingDeg < 0) headingDeg += 360.0;

        o_HeadingDeg = headingDeg;
        return 1; // 궤적 완료, heading 유지
    }

    // 현재 목표 지점
    const auto& target = i_TrajTor[io_NextTorTrajectoryPoint];

    double dE = target.E - curPos.E;
    double dN = target.N - curPos.N;
    double dist = std::sqrt(dE * dE + dN * dN);

    // 도달 판단
    if (dist < reachThreshold) {
        ++io_NextTorTrajectoryPoint;

        // 도달 후 마지막 점 넘어섬 -> heading 유지
        if (io_NextTorTrajectoryPoint >= C_MAX_EXT_TRAJ) {
            const auto& prev = i_TrajTor[C_MAX_EXT_TRAJ - 2];
            const auto& last = i_TrajTor[C_MAX_EXT_TRAJ - 1];

            double dE = last.E - prev.E;
            double dN = last.N - prev.N;

            double headingRad = std::atan2(dE, dN);
            double headingDeg = headingRad * 180.0 / M_PI;
            if (headingDeg < 0) headingDeg += 360.0;

            o_HeadingDeg = headingDeg;
            return 1; // 완료
        }

        // 다음 지점으로 향할 벡터 다시 계산
        const auto& next = i_TrajTor[io_NextTorTrajectoryPoint];
        dE = next.E - curPos.E;
        dN = next.N - curPos.N;
    }

    // 침로 계산
    double headingRad = std::atan2(dE, dN);  // 항법 기준
    double headingDeg = headingRad * 180.0 / M_PI;
    if (headingDeg < 0) headingDeg += 360.0;

    o_HeadingDeg = headingDeg;
    return 0; // 진행 중
}

