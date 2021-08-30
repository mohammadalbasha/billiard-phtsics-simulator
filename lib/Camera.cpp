#include <cmath>
#define pi acos(-1)

class Camera {
    public:
        double posX, posY, posZ;
        double dirX, dirY, dirZ;
        double angleX, angleY, sensitivityFactor, speedFactor; 

        Camera(double posX, double posY, double posZ) {
            this -> posX = posX;
            this -> posY = posY;
            this -> posZ = posZ;
            angleX = angleY = dirX = dirY = dirZ = 0;
            sensitivityFactor = 0.01;
            speedFactor = 0.1;
        }

        void updateAngle(double dx, double dy) {
            angleX -= sensitivityFactor * dx;
            angleY -= sensitivityFactor * dy;

            normalize(angleX);
            normalize(angleY);

            angleY = min(angleY, (pi / 2) - 0.001);
	        angleY =  max(angleY, -(pi / 2) + 0.001);

            dirX = cos(angleX) * cos(angleY);
            dirY = sin(angleY);
            dirZ = -sin(angleX) * cos(angleY);
        }

        void updateAngleAbsolute(double dx, double dy) {
            updateAngle(dx / sensitivityFactor, dy / sensitivityFactor);
        }

        void move() {
            posX += speedFactor * cos(angleX);
            posZ += speedFactor * -sin(angleX);
        }

    private:
        Camera() {}

        void normalize(double &ang) {
            if(ang < 0) ang += floor(-ang / (2 * pi)) * 2 * pi;
            else ang -= floor(ang / (2 * pi)) * 2 * pi;
        }
};