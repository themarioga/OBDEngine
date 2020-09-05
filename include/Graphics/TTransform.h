#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include "TEntity.h"

class TTransform : public TEntity {
    public:
        /**
         * @brief Constructs a new transform entity to be used on a node.
         * Initializes the matrix to an identity one.
         * 
         */
        TTransform();

        /**
         * @brief Destructor for a transform entity.
         * 
         */
        virtual ~TTransform();

        /**
         * @brief stack the current matrix.
         * 
         */
        void beginDraw();
        /**
         * @brief unstack the current matrix on top.
         * 
         */
        void endDraw();

        // Matrix operations
        /**
         * @brief Converts the transform matrix into an identity one.
         * 
         */
        void identity();

        /**
         * @brief overrides the transform matrix with the one passed as
         * parameter.
         * 
         * @param glm::mat4 matrix to replace the current one.
         */
        void load(glm::mat4);

        /**
         * @brief transpose the current matrix
         * 
         */
        void transpose();

        /**
         * @brief transposes the transform matrix, storing in it the result.
         * 
         */
        void invert();

        /**
         * @brief multiplies the transform matrix by a number.
         * 
         * @param f32 number to multiply the transform matrix by.
         */
        void multiply(f32);

        /**
         * @brief mutiplies the transform matrix by a vector3.
         * 
         * @param glm::vec3 vector to mutiply the transform matrix by.
         */
        void multiply(glm::vec3);

        /**
         * @brief mutiplies the matrix by another matrix.
         * 
         * @param glm::mat4 matrix to mutiply the transform matrix by.
         */
        void multiply(glm::mat4);


        // Basic transforms
        /**
         * @brief creates and stores into the transform matrix a translation matrix from the current 
         * one and a vector3 composed of the 3 numbers passed as parameters.
         * 
         * @param glm::vec3 component of the translation.
         */
        void translate(glm::vec3);

        /**
         * @brief creates and stores into the transform matrix a rotation matrix from the current 
         * one and a vector3 composed of the first 3 numbers passed as parameters. The rotation amount is determined
         * by the fourth one, the angle.
         * 
         * @param glm::vec3 component of the rotation.
         * @param f32 angular amount to rotate. This is expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
         */
        void rotate(glm::vec3, f32);

        /**
         * @brief creates and stores into the transform matrix a scale matrix from the current 
         * one and a vector3 composed of the 3 numbers passed as parameters.
         * 
         * @param glm::vec3 component of the scale.
         */
        void scale(glm::vec3);

        /**
         * @brief 
         * 
         * @return glm::mat4 
         */
        glm::mat4 getMatrix();
    private:
        glm::mat4 matrix;
};

#endif