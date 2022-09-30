#ifndef GROOPH_3D_H
#define GROOPH_3D_H

#define GR_OBJ 1

typedef struct {
	unsigned int n_v;
	unsigned int n_vt;
	unsigned int n_vn;
	unsigned int n_f;
	unsigned int n_l;

	double **v;
	double **vt;
	double **vn;
	unsigned int **f;
	unsigned int **l;
} GrAttribute; 

GrAttribute *grCreateAttribute();
void grDestroyAttribute (GrAttribute *attribute);

#endif /* GROOPH_3D_H */
