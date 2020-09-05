#ifndef OBDENTITY_H
#define OBDENTITY_H

#include "OBDTypes.h"
#include "OBDEnums.h"

#include "Graphics/TNode.h"
#include "Graphics/TTransform.h"

class OBDSceneNode;

class OBDEntity {
    public:
		/**
		 * @brief Construct a new OBDEntity object
		 * 
		 * @param parent
		 */
		OBDEntity(OBDSceneNode *);

		/**
		 * @brief Destroy the OBDEntity object
		 * 
		 */
		~OBDEntity();

        /**
         * @brief translate towards the paramter point
         * 
         */
        virtual void translate(glm::vec3 t);

        /**
         * @brief rotate this entity
         * 
         * @param r normalized vector
         * @param a angle
         */
        virtual void rotate(glm::vec3 r, f32 a);

        /**
         * @brief Change the scale of this entity
         * 
         * @param s
         */
        virtual void scale(glm::vec3 s);

        /**
         * @brief Set the Position
         * 
         * @param p 
         */
        virtual void setPosition(glm::vec3 p);

        /**
         * @brief Set the Rotation
         * 
         * @param r 
         */
        virtual void setRotation(glm::vec3 r);

        /**
         * @brief Set the Scale
         * 
         * @param s 
         */
        virtual void setScale(glm::vec3 s);

        /**
         * @brief Set the Active object
         * 
         * @param a 
         */
        virtual void setActive(bool a);

		/**
		 * @brief Get the Position object
		 * 
		 * @return glm::vec3 
		 */
		virtual glm::vec3 getPosition();

		/**
		 * @brief Get the Rotation object
		 * 
		 * @return glm::vec3 
		 */
		virtual glm::vec3 getRotation();

		/**
		 * @brief Get the Scale object
		 * 
		 * @return glm::vec3 
		 */
		virtual glm::vec3 getScale();

        /**
         * @brief Get the Active object
         * 
         * @return true 
         * @return false 
         */
        virtual bool getActive();

		/**
		 * @brief update this entity's model matrix
		 * 
		 * @param modelMatrix
		 */
		virtual void refreshModelMatrix(glm::mat4);

         /**
         * @brief Get the First Node object
         * 
         * @return TNode* 
         */
        virtual TNode *getFirstNode();

         /**
         * @brief Get the First Node object
         * 
         * @return TNode* 
         */
        virtual TNode *getLastNode();
	protected:
		/**
		 * @brief Construct a new OBDEntity object
		 * 
		 */
		OBDEntity();

		OBDSceneNode *parent;

        TNode* rotationNode;
        TNode* translationNode;
        TNode* scaleNode;

        glm::vec3 node_position;
        glm::vec3 node_rotation;
        glm::vec3 node_scale;

		glm::mat4 parent_model_matrix;
		glm::mat4 model_matrix;
		glm::mat4 inverse_model_matrix;
};

#endif