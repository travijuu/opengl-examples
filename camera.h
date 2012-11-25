#ifndef _CAMERA_
#define _CAMERA_

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define _PI_      3.1415926535897932
#define RADIAN    0.0174532925199432

class Camera
{

private:

	float  m_xPosition, m_yPosition, m_zPosition ;
	float  m_LeftRightAngle ;   // angle between x and z userin acisi
	float  m_UpDownAngle  ;   // angle between y and z
    float  m_Speed ;
	float  m_dirX ;//cameranin yonu
    float  m_dirZ ;//cameranin yonu
    float  m_dirY ;

	double  sin_table[360] ;  // hizi artirmak icin tablolar kullanildi.
	double  cos_table[360] ;

	inline void initTables() {
	    //
		// sin ve cos table'larini olustur.
		//
		for ( int i =0; i < 360; i++ )
		{
			sin_table[i] = sin(i*RADIAN);
			cos_table[i] = cos(i*RADIAN);
		}
	}
public:

	Camera()
	{
		Init(0, 0, 0, 0, 0, 5 );
	    Update() ;	
	
	}

	//constructor 
	//cameranin baslangic noktasi ilk uc
	//sonuncusu speed

    inline Camera( float p_x, float p_y, float p_z,float p_lr, float p_ud, float p_spd )
	{
		Init( p_x, p_y, p_z, p_lr, p_ud, p_spd );
	}

	inline void SetPosition( float p_fPosX , float p_fPosY, float p_fPosZ )
	{
		m_xPosition = p_fPosX ;
		m_yPosition = p_fPosY ;
		m_zPosition = p_fPosZ ;
	}

	inline void SetLeftRight( float p_fLR )
	{
		m_LeftRightAngle = p_fLR ;
		Update() ;
	}

	inline void SetUpDown( float p_fUD )
	{
		m_UpDownAngle = p_fUD ;
	}

	inline void SetSpeed( float p_fSpd )
	{
		m_Speed = p_fSpd  ;
	}

	inline void Init( float p_x, float p_y, float p_z,
		       float p_lr, float p_ud, float p_spd )
	{
		SetPosition( p_x, p_y, p_z);
		SetLeftRight( p_lr );
		SetUpDown( p_ud );
		SetSpeed( p_spd );
		initTables();  // sin ve cos tablo olustur.
		Update() ;
	}
	
	//camera nerdeyse spped  kadar ileri gidiyor
	//fonksiyon cagrilmadan direk cagriliyor inline olunca
	inline void Forward()
	{
    //    Update() ;

		m_xPosition +=  m_dirX ;
		m_zPosition -=  m_dirZ ;
        m_yPosition +=  m_dirY ;

	}

	//camera nerdeyse spped  kadar gerý gidiyor
	inline void Backward()
	{
    //    Update() ;

		m_xPosition -= m_dirX   ;
		m_zPosition += m_dirZ   ;
		m_yPosition -= m_dirY ;
	}

	//cameranin ne kadar donmesi
    inline TurnRightLeft( float p_angle )
	{
		m_LeftRightAngle = ( ((int) m_LeftRightAngle  + (int) p_angle)  % 360 ) ;
		if ( m_LeftRightAngle < 0 ) m_LeftRightAngle = 360 + m_LeftRightAngle ;
		
        Update() ;
		
	}
   
	inline TurnUpDown( float p_angle )
	{
	  
		
	  m_UpDownAngle = ( ((int) m_UpDownAngle  + (int) p_angle)  % 360 ) ;
	//  printf("Gercek 1  %f\n", m_UpDownAngle );
	
	/*  if ( m_UpDownAngle >= 90 )
		  m_UpDownAngle = 89 ;

	  /*if ( m_UpDownAngle <= -90 )
		  m_UpDownAngle = -89 ;*/

	  if ( m_UpDownAngle < 0 ) m_UpDownAngle = 360 + m_UpDownAngle ;

//	  printf("Gercek 2  %f\n", m_UpDownAngle );

	  
	  Update() ;
	}

    inline Update()
	{

		m_dirX = m_Speed * sin_table[(int)m_LeftRightAngle]*cos_table[(int)m_UpDownAngle] ; 
		m_dirZ = m_Speed * cos_table[(int)m_LeftRightAngle]*cos_table[(int)m_UpDownAngle] ; 
        m_dirY = m_Speed * sin_table[(int)m_UpDownAngle] ; 

	}

	//bunu cagirnca ona gore ayarliyor
	//býz sadece baslangic noktasini veriyoruz
	inline LookAt()
	{
		gluLookAt( m_xPosition, m_yPosition, m_zPosition,
			       m_xPosition+m_dirX , m_yPosition+m_dirY, m_zPosition-m_dirZ ,
				   0, 1, 0 );
	}
    
    inline float getLeftRight()
	{
		return m_LeftRightAngle ;
	}

	inline Print()
	{
		printf("Position [%.2f  %.2f  %.2f] Angles [%.1f %.1f]  Direction [%.2f  %.2f]\n",
			m_xPosition, m_yPosition, m_zPosition, m_LeftRightAngle, m_UpDownAngle,
			m_dirX, m_dirZ);
	}


	inline Upward()
	{
		m_yPosition += m_Speed ;
	}

	inline Downward()
	{
		m_yPosition -= m_Speed ;
	}


	inline Left()
	{
      // Update() ;

	   m_xPosition -=  m_dirZ   ;
	   m_zPosition -=  m_dirX ;
    }

	inline Right()
	{
      // Update() ;

	   m_xPosition +=  m_dirZ   ;
	   m_zPosition +=  m_dirX ;
    }


} ;




#endif