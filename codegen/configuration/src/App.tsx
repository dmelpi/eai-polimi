import { Fragment, useState, useMemo } from 'react';
import { JsonForms } from '@jsonforms/react';
import Grid from '@mui/material/Grid';
import Button from '@mui/material/Button';
import Typography from '@mui/material/Typography';
import './App.css';
import schema from './schema.json';
import uischema from './uischema.json';
import {
  materialCells,
  materialRenderers,
} from '@jsonforms/material-renderers';
import { makeStyles } from '@mui/styles';

const useStyles = makeStyles({
  container: {
    padding: '1em',
    width: '100%',
  },
  title: {
    textAlign: 'center',
    padding: '0.25em',
  },
  dataContent: {
    display: 'flex',
    justifyContent: 'center',
    borderRadius: '0.25em',
    backgroundColor: '#cecece',
    marginBottom: '1rem',
  },
  resetButton: {
    margin: 'auto !important',
    display: 'block !important',
  },
  saveButton: {
    margin: 'auto !important',
    display: 'block !important',
  },
  processButton: {
    margin: 'auto !important',
    display: 'block !important',
  },
  demoform: {
    margin: 'auto',
    padding: '1rem',
  },
});

const initialData = {};

const renderers = [
  ...materialRenderers,
  //register custom renderers
  
];

const App = () => {
  const classes = useStyles();
  const [data, setData] = useState<any>(initialData);
  const stringifiedData = useMemo(() => JSON.stringify(data, null, 2), [data]);

  const clearData = () => {
    setData({});
  };

  const saveData = () => {
    console.log(data);
    const blob = new Blob([JSON.stringify(data, null, 2)], {type : 'application/json'});
    const a = document.createElement('a');
    a.download = 'params.json';
    a.href = URL.createObjectURL(blob);
    a.addEventListener('click', (e) => {
      setTimeout(() => URL.revokeObjectURL(a.href), 30 * 1000);
    });
    a.click();
  };

  const processData = () => {
    console.log("Processing data");
  };

  return (
    <Fragment>
      <div className='App'>
        <header className='App-header'>
          
          <h1 className='App-title'>Preprocessing pipeline definition for time series data</h1>
          <p className='App-intro'>Define which functions should be applied to your input, then 
          generate code for Python and C.</p>
        </header>
      </div>

      <Grid
        container
        justifyContent={'center'}
        spacing={1}
        className={classes.container}
      >
        <Grid item sm={6}>
          <Typography variant={'h4'} className={classes.title}>
            Parameter JSON file
          </Typography>
          <div className={classes.dataContent}>
            <pre id='boundData'>{stringifiedData}</pre>
          </div>
          <div>
            <Button
              className={classes.resetButton}
              onClick={clearData}
              color='primary'
              variant='contained'
            >
              Clear data
            </Button>
          </div>
          <div>
            <Button
              className={classes.saveButton}
              onClick={saveData}
              color='primary'
              variant='contained'
            >
              Save params.json
            </Button>
          </div>
          <div>
            <Button
              className={classes.processButton}
              onClick={processData}
              color='primary'
              variant='contained'
            >
              Codegen
            </Button>
          </div>
        </Grid>
        <Grid item sm={6}>
          <Typography variant={'h4'} className={classes.title}>
            Preprocessing pipeline definition
          </Typography>
          <div className={classes.demoform}>
            <JsonForms
              schema={schema}
              uischema={uischema}
              data={data}
              renderers={renderers}
              cells={materialCells}
              onChange={({ errors, data }) => setData(data)}
            />
          </div>
        </Grid>
      </Grid>
    </Fragment>
  );
};

export default App;
