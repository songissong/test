#include <iostream>
#include <string>
#include <memory>

// 모든 무장 관리자가 구현해야 할 공통 인터페이스
class IWeaponManager {
public:
    virtual ~IWeaponManager() = default;
    virtual void setOwnShipInfo(const std::string& info) = 0;
};

// 기능별 인터페이스
class ITargetAttacker {
public:
    virtual ~ITargetAttacker() = default;
    virtual void setTarget(const std::string& target) = 0;
};

class IWaypointGuided {
public:
    virtual ~IWaypointGuided() = default;
    virtual void void setWaypoint(const std::string& waypoint) = 0;
};

// -- 기능 컴포넌트 (실제 구현) --
class EngagementCoreComponent {
public:
    void setOwnShipInfo(const std::string& info) {
        std::cout << "Component: 우리 함정 정보 설정 - " << info << std::endl;
    }
};


class TargetAttackComponent {
public:
    void setTarget(const std::string& target) {
        std::cout << "Component: 목표물 설정 - " << target << std::endl;
    }
};

class WaypointGuidedComponent {
public:
    void setWaypoint(const std::string& waypoint) {
        std::cout << "Component: 경유지 설정 - " << waypoint << std::endl;
    }
};

// -- 무장별 매니저 클래스 (인터페이스를 구현하고 컴포넌트를 소유) --

class MissileEngagementManager : public IWeaponManager, public ITargetAttacker {
private:
    EngagementCoreComponent coreComponent_;
    TargetAttackComponent targetComponent_;

public:
    void setOwnShipInfo(const std::string& info) override {
        coreComponent_.setOwnShipInfo(info);
    }
    void setTarget(const std::string& target) override {
        targetComponent_.setTarget(target);
    }
};


class MineEngagementManager : public IWeaponManager, public IWaypointGuided {
private:
    EngagementCoreComponent coreComponent_;
    WaypointGuidedComponent waypointComponent_;

public:
    void setOwnShipInfo(const std::string& info) override {
        coreComponent_.setOwnShipInfo(info);
    }
    void setWaypoint(const std::string& waypoint) override {
        waypointComponent_.setWaypoint(waypoint);
    }
};

class TorpedoEngagementManager : public IWeaponManager, public ITargetAttacker, public IWaypointGuided {
private:
    EngagementCoreComponent coreComponent_;
    TargetAttackComponent targetComponent_;
    WaypointGuidedComponent waypointComponent_;

public:
    void setOwnShipInfo(const std::string& info) override {
        coreComponent_.setOwnShipInfo(info);
    }
    void setTarget(const std::string& target) override {
        targetComponent_.setTarget(target);
    }
    void setWaypoint(const std::string& waypoint) override {
        waypointComponent_.setWaypoint(waypoint);
    }
};


// 팩토리 클래스 (변경 없음)
enum class WeaponType { MISSILE, MINE, TORPEDO };

class WeaponFactory {
public:
    static std::unique_ptr<IWeaponManager> createManager(WeaponType type) {
        switch (type) {
        case WeaponType::MISSILE:
            return std::make_unique<MissileEngagementManager>();
        case WeaponType::MINE:
            return std::make_unique<MineEngagementManager>();
        case WeaponType::TORPEDO:
            return std::make_unique<TorpedoEngagementManager>();
        default: return nullptr;
        }
    }
};

// --- 클라이언트 코드 ---
int main() {
    auto missileManager = WeaponFactory::createManager(WeaponType::MISSILE);
    std::cout << "--- 미사일 매니저 시동 ---" << std::endl;
    missileManager->setOwnShipInfo("미사일 발사함");
    if (auto targetWeapon = dynamic_cast<ITargetAttacker*>(missileManager.get())) {
        targetWeapon->setTarget("적 잠수함");
    }

    std::cout << "\n--- 자항기뢰 매니저 시동 ---" << std::endl;
    auto mineManager = WeaponFactory::createManager(WeaponType::MINE);
    mineManager->setOwnShipInfo("기뢰 부설함");
    if (auto waypointWeapon = dynamic_cast<IWaypointGuided*>(mineManager.get())) {
        waypointWeapon->setWaypoint("경유지 1, 경유지 2");
    }

    std::cout << "\n--- 선유도어뢰 매니저 시동 ---" << std::endl;
    auto torpedoManager = WeaponFactory::createManager(WeaponType::TORPEDO);
    torpedoManager->setOwnShipInfo("어뢰 발사관");
    if (auto targetWeapon = dynamic_cast<ITargetAttacker*>(torpedoManager.get())) {
        targetWeapon->setTarget("적 수상함");
    }
    if (auto waypointWeapon = dynamic_cast<IWaypointGuided*>(torpedoManager.get())) {
        waypointWeapon->setWaypoint("어뢰 경로점");
    }

    return 0;
}

