/*
Bullet Continuous Collision Detection and Physics Library Maya Plugin
Copyright (c) 2008 Herbert Law
 
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising
from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:
 
1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must
not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 
Written by: Herbert Law <Herbert.Law@gmail.com>

Modified by Roman Ponomarev <rponom@gmail.com>
01/22/2010 : Constraints reworked
*/

//hingeConstraintNode.cpp

#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnTransform.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>

#include "rigidBodyNode.h"
#include "hingeConstraintNode.h"
#include "mayaUtils.h"

#include "solver.h"
#include "dSolverNode.h"
#include "constraint/bt_hinge_constraint.h"

MTypeId     hingeConstraintNode::typeId(0x10033B);
MString     hingeConstraintNode::typeName("dHingeConstraint");

MObject     hingeConstraintNode::ia_rigidBodyA;
MObject     hingeConstraintNode::ia_rigidBodyB;
MObject     hingeConstraintNode::ia_damping;
MObject     hingeConstraintNode::ia_lowerLimit;
MObject     hingeConstraintNode::ia_upperLimit;
MObject     hingeConstraintNode::ia_limitSoftness;
MObject     hingeConstraintNode::ia_biasFactor;
MObject     hingeConstraintNode::ia_relaxationFactor;
MObject     hingeConstraintNode::ia_rotationInA;
MObject     hingeConstraintNode::ia_rotationInB;

MObject     hingeConstraintNode::ia_pivotInA;
MObject     hingeConstraintNode::ia_pivotInB;

MObject     hingeConstraintNode::ia_enableAngularMotor;
MObject     hingeConstraintNode::ia_motorTargetVelocity;
MObject     hingeConstraintNode::ia_maxMotorImpulse;

MObject     hingeConstraintNode::ca_constraint;
MObject     hingeConstraintNode::ca_constraintParam;

MStatus hingeConstraintNode::initialize()
{
    MStatus status;
    MFnMessageAttribute fnMsgAttr;
    MFnNumericAttribute fnNumericAttr;
    MFnMatrixAttribute fnMatrixAttr;

    ia_rigidBodyA = fnMsgAttr.create("inRigidBodyA", "inrba", &status);
    MCHECKSTATUS(status, "creating inRigidBodyA attribute")
    status = addAttribute(ia_rigidBodyA);
    MCHECKSTATUS(status, "adding inRigidBodyA attribute")

    ia_rigidBodyB = fnMsgAttr.create("inRigidBodyB", "inrbb", &status);
    MCHECKSTATUS(status, "creating inRigidBodyB attribute")
    status = addAttribute(ia_rigidBodyB);
    MCHECKSTATUS(status, "adding inRigidBodyB attribute")

    ia_damping = fnNumericAttr.create("damping", "dmp", MFnNumericData::kDouble, 1.0, &status);
    MCHECKSTATUS(status, "creating damping attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_damping);
    MCHECKSTATUS(status, "adding damping attribute")

    ia_lowerLimit = fnNumericAttr.create("lowerLimit", "llmt", MFnNumericData::kDouble, 1.0, &status);
    MCHECKSTATUS(status, "creating lower limit attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_lowerLimit);
    MCHECKSTATUS(status, "adding lower limit attribute")

	ia_upperLimit = fnNumericAttr.create("upperLimit", "ulmt", MFnNumericData::kDouble, -1.0, &status);
    MCHECKSTATUS(status, "creating upper limit attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_upperLimit);
    MCHECKSTATUS(status, "adding upper limit attribute")

	ia_limitSoftness = fnNumericAttr.create("limitSoftness", "lmSo", MFnNumericData::kDouble, 0.9, &status);
    MCHECKSTATUS(status, "creating limitSoftness attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_limitSoftness);
    MCHECKSTATUS(status, "adding limitSoftness attribute")

	ia_biasFactor = fnNumericAttr.create("biasFactor", "biFa", MFnNumericData::kDouble, 0.3, &status);
    MCHECKSTATUS(status, "creating biasFactor attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_biasFactor);
    MCHECKSTATUS(status, "adding biasFactor attribute")

	ia_relaxationFactor = fnNumericAttr.create("relaxationFactor", "reFa", MFnNumericData::kDouble, 1.0, &status);
    MCHECKSTATUS(status, "creating relaxationFactor attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_relaxationFactor);
    MCHECKSTATUS(status, "adding relaxationFactor attribute")

	ia_rotationInA = fnNumericAttr.createPoint("rotationInA", "hgRotA", &status);
	status = fnNumericAttr.setDefault((double) 0.0, (double) 0.0, (double) 0.0);
    MCHECKSTATUS(status, "creating rotationInA attribute")
    status = addAttribute(ia_rotationInA);
    MCHECKSTATUS(status, "adding rotationInA attribute")

	ia_rotationInB = fnNumericAttr.createPoint("rotationInB", "hgRotB", &status);
	status = fnNumericAttr.setDefault((double) 0.0, (double) 0.0, (double) 0.0);
    MCHECKSTATUS(status, "creating rotationInB attribute")
    status = addAttribute(ia_rotationInB);
    MCHECKSTATUS(status, "adding rotationInB attribute")

	ia_pivotInA = fnNumericAttr.createPoint("pivotInA", "pivinA", &status);
	status = fnNumericAttr.setDefault((double) 0.0, (double) 0.0, (double) 0.0);
    MCHECKSTATUS(status, "creating pivotInA attribute")
    status = addAttribute(ia_pivotInA);
    MCHECKSTATUS(status, "adding pivotInA attribute")

	ia_pivotInB = fnNumericAttr.createPoint("pivotInB", "pivinB", &status);
	status = fnNumericAttr.setDefault((double) 0.0, (double) 0.0, (double) 0.0);
    MCHECKSTATUS(status, "creating pivotInB attribute")
    status = addAttribute(ia_pivotInB);
    MCHECKSTATUS(status, "adding pivotInB attribute")


	//------------------------------------------------------------------------------
	
	ia_enableAngularMotor = fnNumericAttr.create("enableAngularMotor", "enAM", MFnNumericData::kBoolean, false, &status);
    MCHECKSTATUS(status, "creating enableAngularMotor attribute")
    status = addAttribute(ia_enableAngularMotor);
    MCHECKSTATUS(status, "adding enableAngularMotor attribute")

    ia_motorTargetVelocity = fnNumericAttr.create("motorTargetVelocity", "mTV", MFnNumericData::kDouble, 1, &status);
    MCHECKSTATUS(status, "creating motorTargetVelocity attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_motorTargetVelocity);
    MCHECKSTATUS(status, "adding motorTargetVelocity attribute")

	ia_maxMotorImpulse = fnNumericAttr.create("maxMotorImpulse", "mMI", MFnNumericData::kDouble, 1, &status);
    MCHECKSTATUS(status, "creating maxMotorImpulse attribute")
    fnNumericAttr.setKeyable(true);
    status = addAttribute(ia_maxMotorImpulse);
    MCHECKSTATUS(status, "adding maxMotorImpulse attribute")

	//------------------------------------------------------------------------------

	ca_constraint = fnNumericAttr.create("ca_constraint", "caco", MFnNumericData::kBoolean, 0, &status);
    MCHECKSTATUS(status, "creating ca_constraint attribute")
    fnNumericAttr.setConnectable(false);
    fnNumericAttr.setHidden(true);
    fnNumericAttr.setStorable(false);
    fnNumericAttr.setKeyable(false);
    status = addAttribute(ca_constraint);
    MCHECKSTATUS(status, "adding ca_constraint attribute")

    ca_constraintParam = fnNumericAttr.create("ca_constraintParam", "cacop", MFnNumericData::kBoolean, 0, &status);
    MCHECKSTATUS(status, "creating ca_constraintParam attribute")
    fnNumericAttr.setConnectable(false);
    fnNumericAttr.setHidden(true);
    fnNumericAttr.setStorable(false);
    fnNumericAttr.setKeyable(false);
    status = addAttribute(ca_constraintParam);
    MCHECKSTATUS(status, "adding ca_constraintParam attribute")


    status = attributeAffects(ia_rigidBodyA, ca_constraint);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rigidBodyA, ca_constraint)")

    status = attributeAffects(ia_rigidBodyA, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rigidBodyA, ca_constraintParam)")

    status = attributeAffects(ia_rigidBodyB, ca_constraint);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rigidBodyB, ca_constraint)")

    status = attributeAffects(ia_rigidBodyB, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rigidBodyB, ca_constraintParam)")

    status = attributeAffects(ia_damping, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_damping, ca_constraintParam)")

    status = attributeAffects(ia_lowerLimit, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_lowerLimit, ca_constraintParam)")

	status = attributeAffects(ia_upperLimit, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_upperLimit, ca_constraintParam)")

	status = attributeAffects(ia_limitSoftness, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_limitSoftness, ca_constraintParam)")
	status = attributeAffects(ia_biasFactor, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_biasFactor, ca_constraintParam)")
	status = attributeAffects(ia_relaxationFactor, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_relaxationFactor, ca_constraintParam)")

	status = attributeAffects(ia_rotationInA, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rotationInA, ca_constraintParam)")
	status = attributeAffects(ia_rotationInB, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_rotationInB, ca_constraintParam)")

	status = attributeAffects(ia_pivotInA, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_pivotInA, ca_constraintParam)")
	status = attributeAffects(ia_pivotInB, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_pivotInB, ca_constraintParam)")


	status = attributeAffects(ia_enableAngularMotor, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_enableAngularMotor, ca_constraintParam)")
	status = attributeAffects(ia_motorTargetVelocity, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_motorTargetVelocity, ca_constraintParam)")
	status = attributeAffects(ia_maxMotorImpulse, ca_constraintParam);
    MCHECKSTATUS(status, "adding attributeAffects(ia_maxMotorImpulse, ca_constraintParam)")

	return MS::kSuccess;
}

hingeConstraintNode::hingeConstraintNode()
{
    // std::cout << "hingeConstraintNode::hingeConstraintNode" << std::endl;
}

hingeConstraintNode::~hingeConstraintNode()
{
    // std::cout << "hingeConstraintNode::~hingeConstraintNode" << std::endl;
}

void hingeConstraintNode::nodeRemoved(MObject& node, void *clientData)
{
   // std::cout << "hingeConstraintNode::nodeRemoved" << std::endl;
    MFnDependencyNode fnNode(node);
    hingeConstraintNode *pNode = static_cast<hingeConstraintNode*>(fnNode.userNode());
	if (pNode->m_constraint) 
	{
        bt_hinge_constraint_t* hinge_impl = dynamic_cast<bt_hinge_constraint_t*>(pNode->m_constraint->pubImpl());
		rigid_body_t::pointer rigid_bodyA = pNode->m_constraint->rigid_bodyA();
		if(rigid_bodyA)
		{
			rigid_bodyA->remove_constraint(hinge_impl);
		}
		rigid_body_t::pointer rigid_bodyB = pNode->m_constraint->rigid_bodyB();
		if(rigid_bodyB)
		{
			rigid_bodyB->remove_constraint(hinge_impl);
		}
	}
	constraint_t::pointer constraint = static_cast<constraint_t::pointer>(pNode->m_constraint);
    solver_t::remove_constraint(constraint);
}

void* hingeConstraintNode::creator()
{
    return new hingeConstraintNode();
}


bool hingeConstraintNode::setInternalValueInContext ( const  MPlug & plug,
                                                   const  MDataHandle & dataHandle,
                                                   MDGContext & ctx)
{
   /* if ((plug == pdbFiles) || (plug == ia_scale) || (plug == ia_percent)) {
        m_framesDirty = true;
    } else if(plug == textureFiles) {
        gpufx::m_renderer.setColorTextureDirty();
    }*/
    return false; //setInternalValueInContext(plug,dataHandle,ctx);
}

MStatus hingeConstraintNode::compute(const MPlug& plug, MDataBlock& data)
{
    //std::cout << "hingeConstraintNode::compute: " << plug.name() << std::endl;
    //MTime time = data.inputValue( hingeConstraintNode::inTime ).asTime();
    if(plug == ca_constraint) {
        computeConstraint(plug, data);
    } else if(plug == ca_constraintParam) {
        computeConstraintParam(plug, data);
    } else if(plug.isElement()) {
        if(plug.array() == worldMatrix && plug.logicalIndex() == 0) {
            computeWorldMatrix(plug, data);
        } else {
            return MStatus::kUnknownParameter;
        }
    } else {
        return MStatus::kUnknownParameter;
    }
    return MStatus::kSuccess;
}

void hingeConstraintNode::draw( M3dView & view, const MDagPath &path,
                             M3dView::DisplayStyle style,
                             M3dView::DisplayStatus status )
{
  //  std::cout << "hingeConstraintNode::draw" << std::endl;

    update();

    view.beginGL();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glDisable(GL_LIGHTING);

    if( !(status == M3dView::kActive ||
        status == M3dView::kLead ||
        status == M3dView::kHilite ||
        ( style != M3dView::kGouraudShaded && style != M3dView::kFlatShaded )) ) {
        glColor3f(1.0, 1.0, 0.0); 
    }

	vec3f posA, posB, pivB;
	rigid_body_t::pointer rigid_bodyB = NULL;
	if (m_constraint) 
	{
		vec3f pos;
		quatf rot;
		m_constraint->rigid_bodyA()->get_transform(pos, rot);
		m_constraint->worldToA(pos, posA);
		rigid_bodyB = m_constraint->rigid_bodyB();
		if(rigid_bodyB)
		{
			rigid_bodyB->get_transform(pos, rot);
			m_constraint->worldToA(pos, posB);
		}
		vec3f zeroVec(0.f,0.f,0.f);
		m_constraint->worldFromB(zeroVec, pos);
		m_constraint->worldToA(pos, pivB);
	}

    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(posA[0], posA[1], posA[2]);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(pivB[0], pivB[1], pivB[2]);

	if(rigid_bodyB)
	{
		glVertex3f(pivB[0], pivB[1], pivB[2]);
		glVertex3f(posB[0], posB[1], posB[2]);
	}


    glVertex3f(-1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glVertex3f(0.0, -1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, 1.0);

	vec3f posT, posP, posM;
	
	vec3f minusXVec(-1.f,0.f,0.f);
	vec3f posXVec(1.f,0.f,0.f);
	vec3f minusYVec (0.f,-1.f,0.f);
	vec3f posYVec (0.f,1.f,0.f);
	vec3f minusZVec (0.f,0.f,-1.f);
	vec3f posZVec (0.f,0.f,1.f);
	

	m_constraint->worldFromB(minusXVec, posT);
	m_constraint->worldToA(posT, posM);
	m_constraint->worldFromB(posXVec, posT);
	m_constraint->worldToA(posT, posP);
    glVertex3f(posM[0], posM[1], posM[2]);
    glVertex3f(posP[0], posP[1], posP[2]);
	m_constraint->worldFromB(minusYVec, posT);
	m_constraint->worldToA(posT, posM);
	m_constraint->worldFromB(posYVec, posT);
	m_constraint->worldToA(posT, posP);
    glVertex3f(posM[0], posM[1], posM[2]);
    glVertex3f(posP[0], posP[1], posP[2]);
	m_constraint->worldFromB(minusZVec, posT);
	m_constraint->worldToA(posT, posM);
	m_constraint->worldFromB(posZVec, posT);
	m_constraint->worldToA(posT, posP);
    glVertex3f(posM[0], posM[1], posM[2]);
    glVertex3f(posP[0], posP[1], posP[2]);
	

    glEnd();

    glPopAttrib();
    view.endGL();
}

bool hingeConstraintNode::isBounded() const
{
    //return true;
    return false;
}

MBoundingBox hingeConstraintNode::boundingBox() const
{
    // std::cout << "hingeConstraintNode::boundingBox()" << std::endl;
    //load the pdbs
    MObject node = thisMObject();

    MPoint corner1(-1, -1, -1);
    MPoint corner2(1, 1, 1);
    return MBoundingBox(corner1, corner2);
}

//standard attributes
void hingeConstraintNode::computeConstraint(const MPlug& plug, MDataBlock& data)
{
   // std::cout << "hingeConstraintNode::computeConstraint" << std::endl;

    MObject thisObject(thisMObject());
    MPlug plgRigidBodyA(thisObject, ia_rigidBodyA);
    MPlug plgRigidBodyB(thisObject, ia_rigidBodyB);
    MObject update;
    //force evaluation of the rigidBody
    plgRigidBodyA.getValue(update);
    plgRigidBodyB.getValue(update);

    rigid_body_t::pointer  rigid_bodyA;
    if(plgRigidBodyA.isConnected()) {
        MPlugArray connections;
        plgRigidBodyA.connectedTo(connections, true, true);
        if(connections.length() != 0) {
            MFnDependencyNode fnNode(connections[0].node());
            if(fnNode.typeId() == rigidBodyNode::typeId) {
                rigidBodyNode *pRigidBodyNodeA = static_cast<rigidBodyNode*>(fnNode.userNode());
                rigid_bodyA = pRigidBodyNodeA->rigid_body();    
            } else {
                std::cout << "hingeConstraintNode connected to a non-rigidbody node A!" << std::endl;
            }
        }
    }
    rigid_body_t::pointer  rigid_bodyB;
    if(plgRigidBodyB.isConnected()) {
        MPlugArray connections;
        plgRigidBodyB.connectedTo(connections, true, true);
        if(connections.length() != 0) {
            MFnDependencyNode fnNode(connections[0].node());
            if(fnNode.typeId() == rigidBodyNode::typeId) {
                rigidBodyNode *pRigidBodyNodeB = static_cast<rigidBodyNode*>(fnNode.userNode());
                rigid_bodyB = pRigidBodyNodeB->rigid_body();    
            } else {
                std::cout << "hingeConstraintNode connected to a non-rigidbody node B!" << std::endl;
            }
        }
    }

	vec3f pivInA, pivInB;

	if((rigid_bodyA != NULL) && (rigid_bodyB != NULL))
	{
        constraint_t::pointer constraint = static_cast<constraint_t::pointer>(m_constraint);
        solver_t::remove_constraint(constraint);
		float3& mPivInA = data.inputValue(ia_pivotInA).asFloat3();
		float3& mPivInB = data.inputValue(ia_pivotInB).asFloat3();
		for(int i = 0; i < 3; i++)
		{
			pivInA[i] = (float)mPivInA[i];
			pivInB[i] = (float)mPivInB[i];
		}
		float3& mRotInA = data.inputValue(ia_rotationInA).asFloat3();
        MEulerRotation meulerA(deg2rad(mRotInA[0]), deg2rad(mRotInA[1]), deg2rad(mRotInA[2]));
        MQuaternion mquatA = meulerA.asQuaternion();
		quatf rotA((float)mquatA.w, (float)mquatA.x, (float)mquatA.y, (float)mquatA.z);
		float3& mRotInB = data.inputValue(ia_rotationInB).asFloat3();
        MEulerRotation meulerB(deg2rad(mRotInB[0]), deg2rad(mRotInB[1]), deg2rad(mRotInB[2]));
        MQuaternion mquatB = meulerB.asQuaternion();
		quatf rotB((float)mquatB.w, (float)mquatB.x, (float)mquatB.y, (float)mquatB.z);
        m_constraint = solver_t::create_hinge_constraint(rigid_bodyA, pivInA, rotA, rigid_bodyB, pivInB, rotB);
        constraint = static_cast<constraint_t::pointer>(m_constraint);
        solver_t::add_constraint(constraint);
	}
    else if(rigid_bodyA != NULL) 
	{
        //not connected to a rigid body, put a default one
        constraint_t::pointer constraint = static_cast<constraint_t::pointer>(m_constraint);
        solver_t::remove_constraint(constraint);
		float3& mPivInA = data.inputValue(ia_pivotInA).asFloat3();
		for(int i = 0; i < 3; i++)
		{
			pivInA[i] = (float)mPivInA[i];
		}
		float3& mRotInA = data.inputValue(ia_rotationInA).asFloat3();
        MEulerRotation meuler(deg2rad(mRotInA[0]), deg2rad(mRotInA[1]), deg2rad(mRotInA[2]));
        MQuaternion mquat = meuler.asQuaternion();
		quatf rotA((float)mquat.w, (float)mquat.x, (float)mquat.y, (float)mquat.z);
        m_constraint = solver_t::create_hinge_constraint(rigid_bodyA, pivInA, rotA);
        constraint = static_cast<constraint_t::pointer>(m_constraint);
        solver_t::add_constraint(constraint);
    }

    data.outputValue(ca_constraint).set(true);
    data.setClean(plug);
}

void hingeConstraintNode::computeWorldMatrix(const MPlug& plug, MDataBlock& data)
{
    MObject thisObject(thisMObject());
    MFnDagNode fnDagNode(thisObject);

    MObject update;
    MPlug(thisObject, ca_constraint).getValue(update);
    MPlug(thisObject, ca_constraintParam).getValue(update);

    MStatus status;
    MFnTransform fnParentTransform(fnDagNode.parent(0, &status));
	double fixScale[3] = { 1., 1., 1. };  // lock scale
	fnParentTransform.setScale(fixScale);
    MVector mtranslation = fnParentTransform.getTranslation(MSpace::kTransform, &status);

	if(dSolverNode::isStartTime)
	{	// allow to edit pivots
		MPlug plgRigidBodyA(thisObject, ia_rigidBodyA);
		MPlug plgRigidBodyB(thisObject, ia_rigidBodyB);
		MObject update;
		//force evaluation of the rigidBody
		plgRigidBodyA.getValue(update);
		if(plgRigidBodyA.isConnected()) 
		{
			MPlugArray connections;
			plgRigidBodyA.connectedTo(connections, true, true);
			if(connections.length() != 0) 
			{
				MFnDependencyNode fnNode(connections[0].node());
				if(fnNode.typeId() == rigidBodyNode::typeId) 
				{
					MObject rbAObj = fnNode.object();
					rigidBodyNode *pRigidBodyNodeA = static_cast<rigidBodyNode*>(fnNode.userNode());
					MPlug(rbAObj, pRigidBodyNodeA->worldMatrix).elementByLogicalIndex(0).getValue(update);
				}
			}
		}
		plgRigidBodyB.getValue(update);
		if(plgRigidBodyB.isConnected()) 
		{
			MPlugArray connections;
			plgRigidBodyB.connectedTo(connections, true, true);
			if(connections.length() != 0) 
			{
	            MFnDependencyNode fnNode(connections[0].node());
				if(fnNode.typeId() == rigidBodyNode::typeId) 
				{
					MObject rbBObj = fnNode.object();
					rigidBodyNode *pRigidBodyNodeB = static_cast<rigidBodyNode*>(fnNode.userNode());
					MPlug(rbBObj, pRigidBodyNodeB->worldMatrix).elementByLogicalIndex(0).getValue(update);
				}
			}
		}
		if(m_constraint) 
		{
			MQuaternion mrotation;
			fnParentTransform.getRotation(mrotation, MSpace::kTransform);
			bool doUpdatePivot = m_constraint->getPivotChanged();
			if(!doUpdatePivot)
			{
				vec3f worldP;
				quatf worldR;
				m_constraint->get_world(worldP, worldR);
				float deltaPX = worldP[0] - float(mtranslation.x);
				float deltaPY = worldP[1] - float(mtranslation.y);
				float deltaPZ = worldP[2] - float(mtranslation.z);
				float deltaRX = (float)mrotation.x - worldR[1];
				float deltaRY = (float)mrotation.y - worldR[2];
				float deltaRZ = (float)mrotation.z - worldR[3];
				float deltaRW = (float)mrotation.w - worldR[0];
				float deltaSq = deltaPX * deltaPX + deltaPY * deltaPY  + deltaPZ * deltaPZ 
								+ deltaRX * deltaRX + deltaRY * deltaRY + deltaRZ * deltaRZ + deltaRW * deltaRW;
				doUpdatePivot = (deltaSq > FLT_EPSILON);
			}
			if(doUpdatePivot)
			{
				m_constraint->set_world(vec3f((float) mtranslation[0], (float) mtranslation[1], (float) mtranslation[2]),
										quatf((float)mrotation.w, (float)mrotation.x, (float)mrotation.y, (float)mrotation.z));
				vec3f pivInA, pivInB;
				quatf rotInA, rotInB;
				m_constraint->get_frameA(pivInA, rotInA);
				m_constraint->get_frameB(pivInB, rotInB);
				MDataHandle hPivInA = data.outputValue(ia_pivotInA);
				float3 &ihPivInA = hPivInA.asFloat3();
				MDataHandle hPivInB = data.outputValue(ia_pivotInB);
				float3 &ihPivInB = hPivInB.asFloat3();
				for(int i = 0; i < 3; i++) 
				{ 
					ihPivInA[i] = pivInA[i]; 
					ihPivInB[i] = pivInB[i]; 
				}
				MDataHandle hRotInA = data.outputValue(ia_rotationInA);
				float3 &hrotInA = hRotInA.asFloat3();
				MQuaternion mrotA(rotInA[1], rotInA[2], rotInA[3], rotInA[0]);
				MEulerRotation newrotA(mrotA.asEulerRotation());
				hrotInA[0] = rad2deg((float)newrotA.x);
				hrotInA[1] = rad2deg((float)newrotA.y);
				hrotInA[2] = rad2deg((float)newrotA.z);
				MDataHandle hRotInB = data.outputValue(ia_rotationInB);
				float3 &hrotInB = hRotInB.asFloat3();
				MQuaternion mrotB(rotInB[1], rotInB[2], rotInB[3], rotInB[0]);
				MEulerRotation newrotB(mrotB.asEulerRotation());
				hrotInB[0] = rad2deg((float)newrotB.x);
				hrotInB[1] = rad2deg((float)newrotB.y);
				hrotInB[2] = rad2deg((float)newrotB.z);
				m_constraint->setPivotChanged(false);
			}
		}
	}
	else
	{ // if not start time, lock position and rotation
		if(m_constraint) 
		{
			vec3f worldP;
			quatf worldR;
			m_constraint->get_world(worldP, worldR);
            fnParentTransform.setTranslation(MVector(worldP[0], worldP[1], worldP[2]), MSpace::kTransform);
			fnParentTransform.setRotation(MQuaternion(worldR[1], worldR[2], worldR[3], worldR[0])); 
		}
	}
    data.setClean(plug);
}

void hingeConstraintNode::computeConstraintParam(const MPlug& plug, MDataBlock& data)
{
    //std::cout << "hingeConstraintNode::computeRigidBodyParam data.className=" << std::endl;

    MObject thisObject(thisMObject());
    MObject update;
    
    MPlug(thisObject, ca_constraint).getValue(update);
    if(m_constraint) {
		float damping = (float) data.inputValue(ia_damping).asDouble();
		m_constraint->set_damping(damping);
		float lower = (float) data.inputValue(ia_lowerLimit).asDouble();
		float upper = (float) data.inputValue(ia_upperLimit).asDouble();
		float limit_softness = (float) data.inputValue(ia_limitSoftness).asDouble();
		float bias_factor = (float) data.inputValue(ia_biasFactor).asDouble();
		float relaxation_factor = (float) data.inputValue(ia_relaxationFactor).asDouble();
		m_constraint->set_limit(deg2rad(lower), deg2rad(upper), limit_softness, bias_factor, relaxation_factor);
		bool enable_motor = data.inputValue(ia_enableAngularMotor).asBool();
		float motorTargetVelocity = (float) data.inputValue(ia_motorTargetVelocity).asDouble();
		float maxMotorImpulse = (float) data.inputValue(ia_maxMotorImpulse).asDouble();
		m_constraint->enable_motor(enable_motor, motorTargetVelocity, maxMotorImpulse);
    }

    data.outputValue(ca_constraintParam).set(true);
    data.setClean(plug);
}

hinge_constraint_t::pointer hingeConstraintNode::constraint()
{
 //   std::cout << "hingeConstraintNode::rigid_body" << std::endl;

    MObject thisObject(thisMObject());
    MObject update;
    MPlug(thisObject, ca_constraint).getValue(update);
    MPlug(thisObject, ca_constraintParam).getValue(update);

    return m_constraint;
} 

void hingeConstraintNode::update()
{
    MObject thisObject(thisMObject());

    MObject update;
    MPlug(thisObject, ca_constraint).getValue(update);
    MPlug(thisObject, ca_constraintParam).getValue(update);
    MPlug(thisObject, worldMatrix).elementByLogicalIndex(0).getValue(update);
}
