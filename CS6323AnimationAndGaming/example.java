public void solve(Vector3f goalPosition,  double dt){
    int nJoint = ikChain.size();
    IKJoint ikEffector = getIKEffector();

    //
    // STEP 1:
    //Set the IK Configuration from last known joint configuration of the REAL BONES
    for (int i=0; i &lt; nJoint; i++) {
        IKJoint IJ = ikChain.get(i);
        IJ.copyOrientAndTranslationOfRealJoint();
    }
    int dof = 6;
    float fError = 0f;
    ikSolver.iTries=0;
    int currentJointIndex,
    //current joint being worked on                 iRowJac,                 //Row into jacobian ( x,y,z, axis_x, axis_y, axis_z)                 iColumnJac,             //Column into jacobian ( joint_i/bone_i )                 iIndex;
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------         Vector3f temp = new Vector3f();         Quaternion Q = new Quaternion();         do {             // STEP 2:             //  build ik system using preset configuration             buildIKMatrix();             // Step 3:             //  get Effector Position, vectorToGoal, and distance to Goal             effectorPosition.set      (ikEffector.getPosition());             vecToGoal               = Vector3f.sub(effectorPosition, goalPosition, null);             fError                    = vecToGoal.length();             // Is the effector position too far from the target             if( fError &gt; ikSolver.m_fTargetThreshold   ){
            //We are too far;
            IKJoint curIKBone;
            //----------------------------------------------------------------------------
            // Construct the jacobian ; one column at a time
            //----------------------------------------------------------------------------
            for ( iColumnJac = 0, currentJointIndex = 0; iColumnJac &lt; nJoint; ++iColumnJac, ++currentJointIndex ) {
                //Get the current joint world poisition
                curIKBone = ikChain.get(currentJointIndex);
                curJointLocation = curIKBone.getPosition();
                //Get the rotation axis of this joint
                vecJointToEffector                    = Vector3f.sub(effectorPosition,  curJointLocation, vecJointToEffector);
                vecJointToGoal                        = Vector3f.sub( goalPosition   ,      curJointLocation, vecJointToGoal);
                ikSolver.m_arrAxis[currentJointIndex] = Vector3f.cross(vecJointToGoal, vecJointToEffector, ikSolver.m_arrAxis[currentJointIndex]);

                if(ikSolver.m_arrAxis[currentJointIndex].length() == 0){
                    ikSolver.m_arrAxis[currentJointIndex].set(0.0f, 0.0f, 1.0f);
                }
                ikSolver.m_arrAxis[currentJointIndex].normalise();

                //Calculate change(Derivative) of direction need to force this
                //curIKBone to approach the target; describes how Effector changes with joint i change
                dxEffectorNCurrentJoint = Vector3f.cross(vecJointToEffector, ikSolver.m_arrAxis[iColumnJac], dxEffectorNCurrentJoint);


                //Calculate the Jacobian:
                // e =  f ( q )      { where q is orientation, and e is position }
                // the jacobian is partial derivatives stateing how what changes in orientation will produce changes
                // in position; for joint_i the first 3 elements are the changes in position between joint and effector
                // the last 3 elements are the rotation axis needed to get closer to target
                for( iRowJac=0; iRowJac&lt;3; iRowJac++){
                    ikSolver.m_Jacobian.set(iRowJac, iColumnJac, dxEffectorNCurrentJoint.get(iRowJac) );
                    ikSolver.m_Jacobian.set(iRowJac + 3, iColumnJac, vecJointToGoal.get(iRowJac)  );
                }
            }
            // Jacobian: derivative of: e = f ( q ), however
            // we want:  q
            // thus: q = f`(e)
            // this is achieved with the transpose
            ikSolver.m_JacobianTranspose = ikSolver.m_Jacobian.transpose();

            ikSolver.m_farrError.set(0, 0, vecToGoal.getX());
            ikSolver.m_farrError.set(1, 0, vecToGoal.getY());
            ikSolver.m_farrError.set(2, 0, vecToGoal.getZ());
            ikSolver.m_farrError.set(3, 0, 0.0f );//twist
            ikSolver.m_farrError.set(4, 0, 0.0f );//twist
            ikSolver.m_farrError.set(5, 0, 0.0f );//twist
            //Compute q'
            //   e =  f ( q )
            //   e' =  J ( q' ) , where J is the derivative(jacobian) of f
            //   q' =  J ( e' )
            //Compute Joint velocities
            for (iRowJac = 0; iRowJac &lt; nJoint; ++iRowJac){
                ikSolver.m_QDerivative.set(iRowJac, 0, 0.0f);

                for (iColumnJac=0; iColumnJac &lt; 6; ++iColumnJac){
                    float prev = ikSolver.m_QDerivative.get(iRowJac,0);
                    float product = ikSolver.m_JacobianTranspose.get(iRowJac,iColumnJac)  * ikSolver.m_farrError.get(iColumnJac,0);
                    // The derivate is the sum of all changes of individual joints/bones
                    float globalChange = prev+product;
                    //Dt is the rate at which the application is running
                    ikSolver.m_QDerivative.setIncrement(iRowJac, 0, globalChange * (float) dt);// ikSolver.m_fTargetThreshold );
    updateRealBones();
}
