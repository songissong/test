
    double BrHitPoint = 0.0;
    double RangeHitPoint = 0.0;
    CCalcMethod::GetRangeBearing(m_EPResult.HitPoint.E, m_EPResult.HitPoint.N, RangeHitPoint, BrHitPoint);

    
    double dRp = sqrt(pow(m_TmaValues.dROC, 2.0) + pow(BrHitPoint, 2.0) * pow(sin(m_TmaValues.dBOC * DEG2RAD), 2.0));
    double dTEXA_Xp = sqrt(2.) * 0.5 * dRp;
    double dTEXA_Yp = sqrt(2.) * 0.5 * dRp;

    double dTEXA_Xmp, dTEXA_Xmn, dTEXA_Ymp, dTEXA_Ymn;

    if (fabs(m_Target.Speed) < 1e-6) 
    {
        
        dTEXA_Xmp = m_TmaValues.dSOC * m_EPResult.HitTimeDiff;
        dTEXA_Xmn = -m_TmaValues.dSOC * cos(m_TmaValues.dCOC * DEG2RAD) * m_EPResult.HitTimeDiff;
        dTEXA_Ymp = m_TmaValues.dSOC * m_EPResult.HitTimeDiff * tan(m_TmaValues.dCOC * DEG2RAD);
        dTEXA_Ymn = -m_TmaValues.dSOC * m_EPResult.HitTimeDiff * sin(m_TmaValues.dCOC * DEG2RAD);
    }
    else
    {
        
        dTEXA_Xmp = m_TmaValues.dSOC * m_EPResult.HitTimeDiff;
        dTEXA_Xmn = ((m_Target.Speed - m_TmaValues.dSOC) * cos(m_TmaValues.dCOC * DEG2RAD) - m_Target.Speed) * m_EPResult.HitTimeDiff;
        dTEXA_Ymp = (m_Target.Speed + m_TmaValues.dSOC) * m_EPResult.HitTimeDiff * tan(m_TmaValues.dCOC * DEG2RAD);
        dTEXA_Ymn = (m_Target.Speed - m_TmaValues.dSOC) * m_EPResult.HitTimeDiff * sin(m_TmaValues.dCOC * DEG2RAD);
    }

    
    double dTEXA_P_X[4] = { dTEXA_Xp + dTEXA_Xmp, -dTEXA_Xp + dTEXA_Xmn,
                           -dTEXA_Xp + dTEXA_Xmn, dTEXA_Xp + dTEXA_Xmp };
    double dTEXA_P_Y[4] = { dTEXA_Yp + dTEXA_Ymp, dTEXA_Yp + dTEXA_Ymn,
                           -dTEXA_Yp - dTEXA_Ymn, -dTEXA_Yp - dTEXA_Ymp };

    
    double course_rad = (fabs(m_Target.Speed) < 1e-6) ? 0.0 : m_Target.Course * DEG2RAD;
    double sin_course = sin(course_rad);
    double cos_course = cos(course_rad);

    for (int k = 0; k < 4; k++)
    {
        
        
        
        m_EPResult.TEXA_Points[k].E = (sin_course * dTEXA_P_X[k] + cos_course * dTEXA_P_Y[k]) + m_EPResult.HitPoint.E;
        m_EPResult.TEXA_Points[k].N = (cos_course * dTEXA_P_X[k] - sin_course * dTEXA_P_Y[k]) + m_EPResult.HitPoint.N;
    }
