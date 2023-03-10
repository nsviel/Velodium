#include "Surfacic_segmented.h"

#include "../Target/Ref_Operation.h"
#include "../Target/Reference.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/SpeudoInverse.h"

#include <unsupported/Eigen/NonLinearOptimization>


struct LMFunctor
{
	//R It Is
	Eigen::MatrixXf measuredValues;

	int operator()(const Eigen::VectorXf &P, Eigen::VectorXf &fvec) const
	{

		for (int i = 0; i < values(); i++) {
			float xValue = measuredValues(i, 0);
			float yValue = measuredValues(i, 1);
      float zValue = measuredValues(i, 2);

      float fit = 0;
      int cpt = 0;

      for(int i=0; i<=2; i++){
        for(int j=0; j<=2; j++){
           fit += P(cpt)*pow(yValue,i)*pow(xValue,j);
          cpt++;
        }
      }

      fvec(i) = zValue - fit;
		}
		return 0;
	}

	// Compute the jacobian of the errors
	int df(const Eigen::VectorXf &P, Eigen::MatrixXf &fjac) const
	{
		// 'x' has dimensions n x 1
		// It contains the current estimates for the parameters.

		// 'fjac' has dimensions m x n
		// It will contain the jacobian of the errors, calculated numerically in this case.

		float epsilon;
		epsilon = 1e-5f;

		for (int i = 0; i < P.size(); i++) {
			Eigen::VectorXf xPlus(P);
			xPlus(i) += epsilon;
			Eigen::VectorXf xMinus(P);
			xMinus(i) -= epsilon;

			Eigen::VectorXf fvecPlus(values());
			operator()(xPlus, fvecPlus);

			Eigen::VectorXf fvecMinus(values());
			operator()(xMinus, fvecMinus);

			Eigen::VectorXf fvecDiff(values());
			fvecDiff = (fvecPlus - fvecMinus) / (2.0f * epsilon);

			fjac.block(0, i, values(), 1) = fvecDiff;
		}

		return 0;
	}

	// Number of data points, i.e. values.
	int m;

	// Returns 'm', the number of values.
	int values() const { return m; }

	// The number of parameters, i.e. nbParameters.
	int n;

	// Returns 'n', the number of nbParameters.
	int inputs() const { return n; }

};


//Constructor / Destructor
Surfacic_segmented::Surfacic_segmented(Ref_Operation* opeClass){
  this->refopeManager = opeClass;
  //-------------------------------

  this->m = 2;
  this->n = 2;
  this->iter = 30;

  //-------------------------------
}
Surfacic_segmented::~Surfacic_segmented(){}

//Main function
bool Surfacic_segmented::algo(Collection* collection){
  this->algo_nbP();
  refopeManager->compute_Sphere_IbyR();
  //-------------------------------

  //Compute segmented parameter space
  MatrixXf list_P = algo_ParameterSpace_segmented();

	//Write data on a file
	this->plot_writeOnFile(list_P);

  //Correct cloud
  this->algo_correction(cloud, list_P);

  //-------------------------------
  return true;
}

//Subfunctions
MatrixXf Surfacic_segmented::algo_ParameterSpace_segmented(){
  Reference* refManager = refopeManager->get_refManager();
  list<Collection*>* list_sphere = refManager->get_listSphere();
  MatrixXf list_P(list_sphere->size() - 1, nbP);
  //-------------------------------

  for(int i=0; i<list_sphere->size() - 1; i++){
    vector<vec3> sphere_inf = refopeManager->get_sphereData(i);
    vector<vec3> sphere_sup = refopeManager->get_sphereData(i+1);

    vector<float> R_all, cIt_all, Is_all;
    for(int i=0; i<sphere_inf.size(); i++){
      R_all.push_back(sphere_inf[i].x);
      cIt_all.push_back(sphere_inf[i].y);
      Is_all.push_back(sphere_inf[i].z);
    }
    for(int i=0; i<sphere_sup.size(); i++){
      R_all.push_back(sphere_sup[i].x);
      cIt_all.push_back(sphere_sup[i].y);
      Is_all.push_back(sphere_sup[i].z);
    }

    MatrixXf mat(Is_all.size(),3);
    for(int i=0; i<Is_all.size(); i++){
      mat(i,0) = R_all[i];
      mat(i,1) = cIt_all[i];
      mat(i,2) = Is_all[i];
    }

    LMFunctor functor;
    functor.measuredValues = mat;
    functor.m = Is_all.size();
    functor.n = nbP;

    Eigen::LevenbergMarquardt<LMFunctor, float> lm(functor);
    VectorXf P_o(nbP);

		//Si on commence, on initialize arbitrairement
		if(i == 0){
	    P_o(0)=  0.161508;
	    P_o(1)=   -0.142102;
	    P_o(2)=  0.00382948;
	    P_o(3)=   -0.399134;
	    P_o(4)=    0.417444;
	    P_o(5)=  -0.0100024;
	    P_o(6)=   -0.288874;
	    P_o(7)=   -0.216053;
	    P_o(8)=  0.00552558;
		}
		else{//Sinon on initialise en reprenant les valeurs estimées précédement
			P_o = list_P.row(i-1);
		}

    int status = lm.minimize(P_o);

    MatrixXf J = this->compute_Jacobian(R_all, cIt_all);

    VectorXf P = this->compute_SurfaceRegression(J, R_all, cIt_all, Is_all);

    list_P.row(i) = P_o;

    if(i>list_sphere->size()-10){
      //this->plot_oneSegment(R_all, cIt_all, Is_all, P_o);
    }
  }

  //---------------------------
  return list_P;
}
vec2 Surfacic_segmented::algo_searchSegment(float R){
  float sup = 100, inf = -100;
  float inf_out = -1, sup_out = -1;
  vec2 infsup;
  //-------------------------------

  //Prepare references
  vector<float> IbyR_R = refopeManager->get_Sphere_IbyR_R();

  //Search for inferior and superior
  for(int i=0; i<IbyR_R.size(); i++){
    float value = IbyR_R[i] - R;

    if(value <= 0){
      if(value > inf){
        inf = value;
        inf_out = i;
      }
    }else{
      if(value < sup){
        sup = value;
        sup_out = i;
      }
    }
  }

  //Chack for limits
  if(inf_out == -1){
    inf_out = 1;
    sup_out = 2;
  }
  if(sup_out == -1){
    inf_out = IbyR_R.size()-2;
    sup_out = IbyR_R.size()-1;
  }

  infsup = vec2(inf_out, sup_out);

  //-------------------------------
  return infsup;
}
void Surfacic_segmented::algo_correction(Collection* collection, MatrixXf list_P){
	Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), 0);
	vector<float>& Is = cloud->I;
	vector<float>& dist = cloud->R;
	vector<float>& cIt_all = cloud->cosIt;
  Ic.clear(); Im.clear();
  //---------------------------

  //Correection for all points
  for(int i=0; i<Is.size(); i++){

    vec2 infsup = algo_searchSegment(dist[i]);
    VectorXf P = list_P.row(infsup.x);

    float fit = multivariatePoly(P, cIt_all[i], dist[i]);
    Im.push_back(fit);

    float corr = Is[i] * 0.87 / fit;
    Ic.push_back(corr);
  }

  //---------------------------
  this->repacking();
}
void Surfacic_segmented::repacking(){
  //---------------------------

  for(int i=0; i<Im.size(); i++){
    if(Ic[i] < 0) Ic[i] = 0;
    if(Ic[i] > 1) Ic[i] = 1;
  }

  //---------------------------
}

//Surface fitting
void Surfacic_segmented::algo_nbP(){
  nbP = 0;
  //-------------------------------

  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      nbP++;
    }
  }

  //-------------------------------
}
float Surfacic_segmented::multivariatePoly(VectorXf P, float x, float y){
  float fit = 0;
  int cpt = 0;
  //---------------------------

  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      fit += P(cpt)*pow(x,i)*pow(y,j);
      cpt++;
    }
  }

  //---------------------------
  return fit;
}
MatrixXf Surfacic_segmented::compute_Jacobian(vector<float> R, vector<float> cIt_all){
  MatrixXf J = MatrixXf::Zero(R.size(), nbP);
  //--------------------------

  for(int k=0; k<R.size(); k++){
    float y = R[k];
    float x = cIt_all[k];

    int cpt = 0;
    for(int i=0; i<=m; i++){
      for(int j=0; j<=n; j++){
        J(k, cpt) = pow(x,i)*pow(y,j);
        cpt++;
      }
    }
  }

  //---------------------------
  return J;
}
VectorXf Surfacic_segmented::compute_SurfaceRegression(MatrixXf J, vector<float> R, vector<float> cIt_all, vector<float> Is){
  VectorXf E = VectorXf::Zero(R.size());
  VectorXf P = VectorXf::Zero(nbP);
  SpeudoInverse invManager;
  //---------------------------

  for(int i=0; i<iter; i++){
    for(int j=0; j<R.size(); j++){
      float y = R[j];
      float x = cIt_all[j];
      float z = Is[j];

      //Fitting
      float fit = multivariatePoly(P, x, y);

      //Error vectors
      E(j) = fit - z;
    }

    //Optimization
    MatrixXf A, B, C, J_t;
    J_t = J.transpose();
    A = (J_t * J);
    B = (J_t * E);
    C = A.inverse();
  //  C = invManager.SpeudoInverse_orthoDecomp(A);

    P = P - C*B;
  }

  //---------------------------
  return P;
}

//Plotting
void Surfacic_segmented::plot_oneSegment(vector<float>& dist, vector<float>& cIt_all, vector<float>& Is, VectorXf P){
  Plotting plotManager;
  vector<float> map_fit;
  //-------------------------------

  for(int i=0; i<Is.size(); i++){

    float fit = multivariatePoly(P, cIt_all[i], dist[i]);
    map_fit.push_back(fit);
  }

  plotManager.set_namePlot("3Dmap");
  plotManager.set_Xlabel("Distance R (m)");
  plotManager.set_Ylabel("cos({/Symbol a})");
  plotManager.set_Zlabel("Intensity [0;1]");
  plotManager.set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' title 'Raw data'");
  plotManager.set_Format_data2("with pm3d title 'Regression'");
  plotManager.plot_3DmapRegression(dist, cIt_all, Is, map_fit);

  //-------------------------------
}
void Surfacic_segmented::plot_SurfaceFitting(){
  Plotting plotManager;
  vector<float> I01_tot, R_tot, cIt_tot;
  vector<float> map_fit;
  float fit;
  //-------------------------------

  //Get data
  this->algo_nbP();
  MatrixXf list_P = algo_ParameterSpace_segmented();
  refopeManager->compute_ParameterSpace_Sphere(2);
  vector<vec3> PS_Sphere = refopeManager->get_ParameterSpace(); //vector<vec3> : [Is][cosIt][dist]

  //Compute for each segment
  for(int i=0; i<PS_Sphere.size(); i++){

    vec2 infsup = algo_searchSegment(PS_Sphere[i].z);
    VectorXf P = list_P.row(infsup.x);

    fit = multivariatePoly(P, PS_Sphere[i].y, PS_Sphere[i].z);

    I01_tot.push_back(PS_Sphere[i].x);
    cIt_tot.push_back(PS_Sphere[i].y);
    R_tot.push_back(PS_Sphere[i].z);
    map_fit.push_back(PS_Sphere[i].x/fit);
  }

  plotManager.set_Xlabel("Distance R (m)");
  plotManager.set_Ylabel("cos({/Symbol a})");
  plotManager.set_Zlabel("raw [0;1]");
  plotManager.plot_3DmapOnlyRegression(R_tot, cIt_tot, I01_tot);

  plotManager.set_Xlabel("Distance R (m)");
  plotManager.set_Ylabel("cos({/Symbol a})");
  plotManager.set_Zlabel("fitted [0;1]");
  plotManager.plot_3DmapOnlyRegression(R_tot, cIt_tot, map_fit);

  //-------------------------------
}
void Surfacic_segmented::plot_writeOnFile(MatrixXf list_P){
	ofstream file;
	file.open ("../../data/data/SurfacicSegmented_listP.txt");
	vector<float> IbyR_R = refopeManager->get_Sphere_IbyR_R();
	//-------------------------------

  file << "A "<<"B "<<"C "<<"D "<<"E "<<"F "<<"G "<<"H "<<"I "<<"J "<<"K "<<"L "<<"M "<<"\n";
  file <<std::fixed;

	for(int i=0; i<list_P.rows(); i++){
		for(int j=0; j<9; j++){
			file <<setprecision(5)<< list_P(i,j) << " ";
		}

		file <<IbyR_R[i]<<" "<<IbyR_R[i+1]<<" ";
		file <<0.15 << " "<<1;
		file <<"\n";
	}

	//-------------------------------
	file.close();
}
